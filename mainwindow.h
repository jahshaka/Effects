#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QListWidget>
#include <QMainWindow>
#include <QGraphicsPathItem>
#include <QGraphicsView>
#include <QTextEdit>
#include <QDockWidget>
#include "propertylistwidget.h"
//#include "nodemodel.h"
#include "graphicsview.h"

namespace Ui {
class MainWindow;
}

struct nodeListModel {
	QString name;
	//NodeType type;
	int inSockets;
	int outSockets;

};

class SceneWidget;
class GraphNodeScene;
class NodeGraph;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void setNodeGraph(NodeGraph* graph);
    void newNodeGraph();

    ~MainWindow();

private:
    void saveGraph();
    void loadGraph();
    void exportGraph();
    void restoreGraphPositions(const QJsonObject& data);

	void configureUI();
	void generateTileNode();
	void generateTileNode(QList<QListWidgetItem*> list);

	bool eventFilter(QObject *watched, QEvent *event);

    GraphNodeScene* createNewScene();

private:
    Ui::MainWindow *ui;
    GraphNodeScene* scene;
    SceneWidget* sceneWidget;
	NodeGraph *graph;

	QDockWidget* nodeTray;
	QWidget *centralWidget;
	QDockWidget* textWidget;
	QDockWidget* displayWidget;

	QDockWidget *propertyWidget;
	QDockWidget *materialSettingsWidget;
	QTabWidget *tabbedWidget;
	GraphicsView* graphicsView;
	QTextEdit* textEdit;
	PropertyListWidget* propertyListWidget;
	QListWidget *nodeContainer;

	void configureStyleSheet();
};
#endif // MAINWINDOW_H
