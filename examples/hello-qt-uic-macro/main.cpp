#include <QApplication>
#include <QDialog>

#include "ui_gotocelldialog.h"


// linklib Qt5Core
// linklib Qt5Widgets
// linklib Qt5Gui



// cpps-macro qt-uic gotocelldialog.ui

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Ui::GoToCellDialog ui;
    QDialog *dialog = new QDialog;
    ui.setupUi(dialog);
    dialog->show();

    return app.exec();
}
