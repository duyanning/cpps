#include <QApplication>
#include <QLabel>

// vc-extra-compile-flags: -IF:\vcpkg\installed\x86-windows\include\QtWidgets
// 不用上面这行也行，你也可以在config.txt中来句include-dir = F:\vcpkg\installed\x86-windows\include\QtWidgets

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QLabel *label = new QLabel("Hello Qt!");
    label->show();
    return app.exec();
}
