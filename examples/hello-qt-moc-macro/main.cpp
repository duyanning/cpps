#include <QApplication>

#include "finddialog.h"  // usingcpp

// linklib Qt5Core
// linklib Qt5Widgets
// linklib Qt5Gui

// cpps-macro qt-moc finddialog.h

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    FindDialog *dialog = new FindDialog;
    dialog->show();
    return app.exec();
}
