#include <QApplication>
#include <QDialog>

#include "ui_gotocelldialog.h"

// vc-extra-compile-flags: -IF:\vcpkg\installed\x86-windows\include\QtWidgets

/* cpps-make
ui_gotocelldialog.h : gotocelldialog.ui
	uic gotocelldialog.ui -o gotocelldialog.h
	del ui_gotocelldialog.h > NUL 2>&1
	ren gotocelldialog.h ui_gotocelldialog.h
*/
// 上面这是多行make。但这个例子根本不需要这么复杂，用单行make即可。我只是为了举个例子。//之后那个-是为了避免cpps将其当作指令
//- cpps-make ui_gotocelldialog.h : gotocelldialog.ui // uic gotocelldialog.ui -o ui_gotocelldialog.h

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Ui::GoToCellDialog ui;
    QDialog *dialog = new QDialog;
    ui.setupUi(dialog);
    dialog->show();

    return app.exec();
}
