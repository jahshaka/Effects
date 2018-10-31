#include "mainwindow.h"
#include <QApplication>


// https://adared.ch/qnodeseditor-qt-nodesports-based-data-processing-flow-editor/
int main(int argc, char *argv[])
{
  
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
	QApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
	QApplication a(argc, argv);


	MainWindow w;
	w.show();

    return a.exec();
}
