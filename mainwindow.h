#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QListWidget>
#include <QMainWindow>
#include <QGraphicsPathItem>
#include <QGraphicsView>
#include <QTextEdit>
#include <QDockWidget>
#include <QSplitter>
#include "propertylistwidget.h"
//#include "nodemodel.h"
#include "graph/graphicsview.h"
#include "materialsettingswidget.h"
#include "dialogs/createnewdialog.h"
#include "listwidget.h"

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
class NodeLibraryItem;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void setNodeGraph(NodeGraph* graph);
    void newNodeGraph();

    ~MainWindow();

	QList<nodeGraphPreset> list;
private:
    void saveGraph();
    void loadGraph();
    void exportGraph();
    void restoreGraphPositions(const QJsonObject& data);

	void configureUI();
	void generateTileNode();
	void generateTileNode(QList<NodeLibraryItem*> list);
	void addTabs();
	void setNodeLibraryItem(QListWidgetItem *item, NodeLibraryItem *tile);
	void createNewGraph();

	bool eventFilter(QObject *watched, QEvent *event);

    GraphNodeScene* createNewScene();

private:
    Ui::MainWindow *ui;
    GraphNodeScene* scene;
    SceneWidget* sceneWidget;
	NodeGraph *graph;
	QSplitter *splitView;
	

	QDockWidget* nodeTray;
	QWidget *centralWidget;
	QDockWidget* textWidget;
	QDockWidget* displayWidget;
	MaterialSettingsWidget *materialSettingsWidget;

	QDockWidget *propertyWidget;
	QDockWidget *materialSettingsDock;
	QDockWidget *projectDock;
	QDockWidget *assetsDock;
	QTabWidget *tabbedWidget;
	GraphicsView* graphicsView;
	QTextEdit* textEdit;
	PropertyListWidget* propertyListWidget;
	QListWidget *nodeContainer;
	QMenuBar *bar;  
	QMenu *file;
	QMenu *window;
	QMenu *edit;
	QFont font;

	ListWidget *presets;
	ListWidget *effects;

	void configureStyleSheet();
	void configureProjectDock();
	void configureAssetsDock();
};
#endif // MAINWINDOW_H
