#include <QApplication>
#include <QDialog>

#include "gotocelldialog.h"

// cpps-make ui_gotocelldialog.h : gotocelldialog.ui // uic gotocelldialog.ui -o gotocelldialog.h

// vc-extra-compile-flags: -IF:\vcpkg\installed\x86-windows\include\QtWidgets

/* cpps-make
ui_gotocelldialog.h : gotocelldialog.ui
	uic gotocelldialog.ui -o gotocelldialog.h
*/

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Ui::GoToCellDialog ui;
    QDialog *dialog = new QDialog;
    ui.setupUi(dialog);
    dialog->show();

    return app.exec();
}
