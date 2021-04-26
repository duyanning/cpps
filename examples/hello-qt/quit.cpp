#include <QApplication>
#include <QPushButton>

//此行作废// vc-extra-compile-flags: -IF:\vcpkg\installed\x86-windows\include\QtWidgets

// 需要config.txt中设置好头文件跟库的路径。

// linklib Qt5Widgets
// linklib Qt5Core

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QPushButton *button = new QPushButton("Quit");
    QObject::connect(button, SIGNAL(clicked()),
                     &app, SLOT(quit()));
    button->show();
    return app.exec();
}
