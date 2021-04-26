#include <QApplication>

#include "finddialog.h"  // usingcpp

//此行作废// vc-extra-compile-flags: -IF:\vcpkg\installed\x86-windows\include\QtWidgets

// 需要config.txt中设置好头文件跟库的路径。

// linklib Qt5Widgets
// linklib Qt5Core

// using nocheck finddialog.h.moc.cpp
// cpps-make finddialog.h.moc.cpp : finddialog.h // moc finddialog.h -o finddialog.h.moc.cpp
// 上面这是单行make。只支持一条命令。命令写在//之后

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    FindDialog *dialog = new FindDialog;
    dialog->show();
    return app.exec();
}
