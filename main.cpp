#include "mainwindow.h"
#include <QApplication>


// https://adared.ch/qnodeseditor-qt-nodesports-based-data-processing-flow-editor/
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
