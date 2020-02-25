#ifndef NODE_H
#define NODE_H

#include <QApplication>
#include <QColor>
#include <QGraphicsItem>
#include <QSet>

class Link;

class Node : public QGraphicsItem
{
    Q_DECLARE_TR_FUNCTIONS(Node) // 因为QGraphicsItem并不是QObject的派生类

public:
    Node();
    ~Node();

    void setText(const QString &text);
    QString text() const;
    void setTextColor(const QColor &color);
    QColor textColor() const;
    void setOutlineColor(const QColor &color);
    QColor outlineColor() const;
    void setBackgroundColor(const QColor &color);
    QColor backgroundColor() const;

    void addLink(Link *link);
    void removeLink(Link *link);

    // 每个item都得重写这个这个函数，在本例中，约束框包括了边框与边框之外的白边
    QRectF boundingRect() const;
    // 这也是个虚函数，默认是返回boundingRect的结果。本例节点是圆角矩形
    QPainterPath shape() const;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value);

private:
    QRectF outlineRect() const;
    int roundness(double size) const;

    QSet<Link *> myLinks;
    QString myText;
    QColor myTextColor;
    QColor myBackgroundColor;
    QColor myOutlineColor;
};

#endif
