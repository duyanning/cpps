#include <QApplication>

#include "finddialog.h"  // usingcpp

//��������// vc-extra-compile-flags: -IF:\vcpkg\installed\x86-windows\include\QtWidgets

// ��Ҫconfig.txt�����ú�ͷ�ļ������·����

// linklib Qt5Widgets
// linklib Qt5Core

// using nocheck finddialog.h.moc.cpp
// cpps-make finddialog.h.moc.cpp : finddialog.h // moc finddialog.h -o finddialog.h.moc.cpp
// �������ǵ���make��ֻ֧��һ���������д��//֮��

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    FindDialog *dialog = new FindDialog;
    dialog->show();
    return app.exec();
}
