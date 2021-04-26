#include <QApplication>
#include <QPushButton>

//��������// vc-extra-compile-flags: -IF:\vcpkg\installed\x86-windows\include\QtWidgets

// ��Ҫconfig.txt�����ú�ͷ�ļ������·����

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
