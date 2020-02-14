#include <QApplication>

#include "finddialog.h"  // usingcpp

// linklib Qt5Core
// linklib Qt5Widgets
// linklib Qt5Gui

// using nocheck $(SHADOWDIR)/finddialog.h.moc.cpp
// cpps-make $(SHADOWDIR)/finddialog.h.moc.cpp : finddialog.h // moc finddialog.h -o $(SHADOWDIR)/finddialog.h.moc.cpp

// 上边使用了变量$(SHADOWDIR)，该变量代表当前.cpp文件所在目录的影子目录。
// 将moc生成的文件放在影子目录中，就不会弄脏源文件所在目录。

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    FindDialog *dialog = new FindDialog;
    dialog->show();
    return app.exec();
}
