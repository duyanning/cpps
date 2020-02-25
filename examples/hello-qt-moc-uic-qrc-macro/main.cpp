#include <QApplication>

#include "diagramwindow.h" // usingcpp

// linklib Qt5Core
// linklib Qt5Widgets
// linklib Qt5Gui

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    DiagramWindow view;
    view.show();
    return app.exec();
}
