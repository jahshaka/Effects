#pragma once

#include <QListWidget>
#include <QMainWindow>
#include <QWidget>
#include <QGraphicsPathItem>
#include <QGraphicsView>
#include <QTextEdit>
#include <QDockWidget>
#include <QSplitter>
#include <QToolbar>

#include "propertylistwidget.h"
//#include "nodemodel.h"
#include "graph/graphicsview.h"
#include "materialsettingswidget.h"
#include "dialogs/createnewdialog.h"
#include "listwidget.h"
#include "misc/QtAwesome.h"
#include "misc/QtAwesomeAnim.h"
#include "shaderassetwidget.h"


class QMenuBar;
class SceneWidget;
class GraphNodeScene;
class NodeGraph;
class NodeLibraryItem;
class Database;

namespace shadergraph
{

namespace Ui {
class MainWindow;
}

struct nodeListModel {
	QString name;
	//NodeType type;
	int inSockets;
	int outSockets;

};
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow( QWidget *parent = Q_NULLPTR, Database *database = Q_NULLPTR);
    void setNodeGraph(NodeGraph* graph);
    void newNodeGraph(QString *shaderName = Q_NULLPTR, int *templateType = Q_NULLPTR, QString *templateName = Q_NULLPTR);
	
	void refreshShaderGraph();
	void setAssetWidgetDatabase(Database *db);

    ~MainWindow();

	QList<nodeGraphPreset> list;
private:
    void saveGraph();
    void loadGraph();
    void exportGraph();
    void restoreGraphPositions(const QJsonObject& data);

	void configureUI();
	void configureToolbar();
	void generateTileNode();
	void generateTileNode(QList<NodeLibraryItem*> list);
	void addTabs();
	void setNodeLibraryItem(QListWidgetItem *item, NodeLibraryItem *tile);
	void createNewGraph();
	void updateAssetDock();

	bool eventFilter(QObject *watched, QEvent *event);

    GraphNodeScene* createNewScene();
	void regenerateShader();
	
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
	QToolBar *toolBar;
	QMenu *file;
	QMenu *window;
	QMenu *edit;
	QFont font;

	ListWidget *presets;
	ListWidget *effects;
	ShaderAssetWidget *assetWidget;

	void configureStyleSheet();
	void configureProjectDock();
	void configureAssetsDock();
	void createShader(QString *shaderName = Q_NULLPTR, int *templateType = Q_NULLPTR, QString *templateName = Q_NULLPTR);

	QtAwesome *fontIcons;
	Database *dataBase;
	QSize defaultGridSize = QSize(110, 110);
	QSize defaultItemSize = QSize(90, 90);
	QSize defaultPresetsSize = QSize(100, 100);

};

}