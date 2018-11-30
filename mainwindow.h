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

#if(EFFECT_BUILD_AS_LIB)
#include "shaderassetwidget.h"
#endif

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
	void saveShader(QListWidgetItem *item);
	void loadShader();
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

	void configureStyleSheet();
	void configureProjectDock();
	void configureAssetsDock();
	void createShader(QString *shaderName = Q_NULLPTR, int *templateType = Q_NULLPTR, QString *templateName = Q_NULLPTR);

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

	QtAwesome *fontIcons;
	QSize defaultGridSize = QSize(70, 70);
	QSize defaultItemSize = QSize(90, 90);

	QLineEdit *projectName;
#if(EFFECT_BUILD_AS_LIB)
	ShaderAssetWidget *assetWidget;
	Database *dataBase;
#endif
};

}