#include <QtWidgets>
//#include <iostream>
#include "link.h"
#include "node.h"



Node::Node()
{
    myTextColor = Qt::darkGreen;
    myOutlineColor = Qt::darkBlue;
    myBackgroundColor = Qt::white;

    setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges);
}

Node::~Node()
{
    // 边是从属于节点的
    foreach (Link *link, myLinks)
        delete link;
}

void Node::setText(const QString &text)
{
    prepareGeometryChange(); // 文本变了可能会导致节点大小的变化
    myText = text;
    update();
}

QString Node::text() const
{
    return myText;
}

void Node::setTextColor(const QColor &color)
{
    myTextColor = color;
    update();
}

QColor Node::textColor() const
{
    return myTextColor;
}

void Node::setOutlineColor(const QColor &color)
{
    myOutlineColor = color;
    update();
}

QColor Node::outlineColor() const
{
    return myOutlineColor;
}

void Node::setBackgroundColor(const QColor &color)
{
    myBackgroundColor = color;
    update();
}

QColor Node::backgroundColor() const
{
    return myBackgroundColor;
}

void Node::addLink(Link *link)
{
    myLinks.insert(link);
}

void Node::removeLink(Link *link)
{
    myLinks.remove(link);
}

QRectF Node::boundingRect() const
{
    const int Margin = 1;
    // 在边框之外再加上白边
    return outlineRect().adjusted(-Margin, -Margin, +Margin, +Margin);
}

QPainterPath Node::shape() const
{
    QRectF rect = outlineRect();

    QPainterPath path;
    path.addRoundRect(rect, roundness(rect.width()),
                      roundness(rect.height()));
    return path;
}

void Node::paint(QPainter *painter,
                 const QStyleOptionGraphicsItem *option,
                 QWidget * /* widget */)
{
    QPen pen(myOutlineColor);
    if (option->state & QStyle::State_Selected) { // 选中节点的边框为虚线
        pen.setStyle(Qt::DotLine);
        pen.setWidth(2);
    }
    painter->setPen(pen);
    painter->setBrush(myBackgroundColor);

    QRectF rect = outlineRect();
    painter->drawRoundRect(rect, roundness(rect.width()),
                           roundness(rect.height()));

    painter->setPen(myTextColor);
    painter->drawText(rect, Qt::AlignCenter, myText);
}

// 双击编辑节点文字
void Node::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QString text = QInputDialog::getText(event->widget(),
                           tr("Edit Text"), tr("Enter new text:"),
                           QLineEdit::Normal, myText);
    if (!text.isEmpty())
        setText(text);
}

QVariant Node::itemChange(GraphicsItemChange change,
                          const QVariant &value)
{
    //qDebug() << change << '\n';
    if (change == ItemPositionHasChanged) {
        foreach (Link *link, myLinks)
            link->trackNodes(); // 看意思是移动节点后，边会跟着动。但实测发现边并没跟着动
        // 发现原来是移动节点时，本函数并未被调用。原来在Qt 5.9.2中需要设置ItemSendsGeometryChanges才行
        // 参考：https://stackoverflow.com/questions/8187807/itemchanged-never-called-on-qgraphicsitem
    }
    return QGraphicsItem::itemChange(change, value);
}

QRectF Node::outlineRect() const
{
    const int Padding = 8;
    QFontMetricsF metrics = (QFontMetricsF)qApp->font();
    QRectF rect = metrics.boundingRect(myText);
//    qDebug() << "rect1:" <<  rect << '\n';
    // 文字框四周加上填充
    rect.adjust(-Padding, -Padding, +Padding, +Padding);
//    qDebug() << "rect2:" <<  rect << '\n';
    rect.translate(-rect.center());  // 将矩形四个角的坐标减去矩形中心的坐标(鬼知道此时原点在哪里)，得到的四个坐标是以矩形中心为原点的
//    qDebug() << "rect3:" <<  rect << '\n';
    return rect;
}

int Node::roundness(double size) const
{
    const int Diameter = 12;
    return 100 * Diameter / int(size);
}
