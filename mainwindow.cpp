#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "nodegraph.h"
//#include "nodemodel.h"
#include <QMouseEvent>
#include <QDebug>
#include "scenewidget.h"
#include <QLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QFile>
#include <QByteArray>
//#include "graphtest.h"
#include "generator/shadergenerator.h"
#include "nodes/test.h"
#include "materialwriter.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	configureUI();

    scene = nullptr;
	sceneWidget = new SceneWidget();

    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::saveGraph);
	connect(ui->actionLoad, &QAction::triggered, this, &MainWindow::loadGraph);
	connect(ui->actionExport, &QAction::triggered, this, &MainWindow::exportGraph);

    // preview widget
 /*   sceneWidget = new SceneWidget();
    auto grid = new QGridLayout();
    grid->setSpacing(0);
    grid->setMargin(0);
    grid->addWidget(sceneWidget);
    ui->sceneContainer->setLayout(grid);*/

    // add menu items to property widget
    newNodeGraph();
}

void MainWindow::setNodeGraph(NodeGraph *graph)
{
    // create and set new scene
    auto newScene = createNewScene();
   // ui->graphicsView->setScene(newScene);
	graphicsView->setScene(newScene);
	graphicsView->setAcceptDrops(true);
    newScene->setNodeGraph(graph);

    // delet old scene and reassign new scene
    if (scene) {
        scene->deleteLater();
    }
    scene = newScene;

    //ui->propertyContainerPage1->setNodeGraph(graph);
	propertyListWidget->setNodeGraph(graph);
    sceneWidget->setNodeGraph(graph);
	displayWidget->setWidget(sceneWidget);
	this->graph = graph;
}

void MainWindow::newNodeGraph()
{
    auto graph = new NodeGraph;
    registerModels(graph);
    auto masterNode = new SurfaceMasterNode();
    graph->addNode(masterNode);
    graph->setMasterNode(masterNode);

    setNodeGraph(graph);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::saveGraph()
{
    QString path = QFileDialog::getSaveFileName(this, "Choose file name","material.json","Material File (*.json)");

    QJsonDocument doc;
    doc.setObject(scene->serialize());

    QFile file(path);
    file.open(QFile::WriteOnly | QFile::Truncate);
    file.write(doc.toJson());
    file.close();
}

void MainWindow::loadGraph()
{
    QString path = QFileDialog::getOpenFileName(this, "Choose file name","material.json","Material File (*.json)");

    QFile file;
    file.setFileName(path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    auto val = file.readAll();
    file.close();
    QJsonDocument d = QJsonDocument::fromJson(val);

    auto graph = NodeGraph::deserialize(d.object());
    this->setNodeGraph(graph);
    this->restoreGraphPositions(d.object());
}

void MainWindow::exportGraph()
{
	QString path = QFileDialog::getSaveFileName(this, "Choose file name", "material.material", "Material File (*.shader)");

	QJsonDocument doc;
	doc.setObject((new MaterialWriter())->serializeMaterial(graph));

	QFile file(path);
	file.open(QFile::WriteOnly | QFile::Truncate);
	file.write(doc.toJson());
	file.close();

	QString sourcePath = QFileInfo(path).absolutePath()+"/shader.frag";
	QFile sourceFile(sourcePath);
	sourceFile.open(QFile::WriteOnly | QFile::Truncate);
	sourceFile.write("#pragma include <surface.frag>\n\n"+(new ShaderGenerator())->generateShader(graph).toUtf8());
	sourceFile.close();
}

void MainWindow::restoreGraphPositions(const QJsonObject &data)
{
    auto scene = data["scene"].toObject();
    auto nodeList = scene["nodes"].toArray();

    for(auto nodeVal : nodeList) {
        auto nodeObj = nodeVal.toObject();
        auto nodeId = nodeObj["id"].toString();
        auto node = this->scene->getNodeById(nodeId);
        node->setX(nodeObj["x"].toDouble());
        node->setY(nodeObj["y"].toDouble());
    }
}

void MainWindow::configureUI()
{
	nodeTray = new QDockWidget("node tray");
	centralWidget = new QWidget();
	textWidget = new QDockWidget("code view");
	displayWidget = new QDockWidget("display");

	propertyWidget = new QDockWidget("properties");
	materialSettingsWidget = new QDockWidget("material settings");
	tabbedWidget = new QTabWidget;
	graphicsView = new QGraphicsView;
	textEdit = new QTextEdit;
	propertyListWidget = new PropertyListWidget;
	nodeContainer = new QListWidget;


	nodeTray->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	textWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	displayWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	propertyWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	materialSettingsWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

	setDockNestingEnabled(true);
	this->setCentralWidget(graphicsView);
	addDockWidget(Qt::LeftDockWidgetArea, nodeTray, Qt::Vertical);
	addDockWidget(Qt::RightDockWidgetArea, textWidget, Qt::Vertical);
	addDockWidget(Qt::RightDockWidgetArea, displayWidget, Qt::Vertical);
	addDockWidget(Qt::RightDockWidgetArea, materialSettingsWidget, Qt::Vertical);
	addDockWidget(Qt::RightDockWidgetArea, propertyWidget, Qt::Vertical);
	QMainWindow::tabifyDockWidget(propertyWidget, materialSettingsWidget);

	textWidget->setWidget(textEdit);
	propertyWidget->setWidget(propertyListWidget);
	nodeTray->setWidget(nodeContainer);
	QSize currentSize(50, 50);


	nodeContainer->setViewMode(QListWidget::IconMode);
	nodeContainer->setIconSize(currentSize);
	nodeContainer->setMouseTracking(true);
	nodeContainer->setAlternatingRowColors(true);
	nodeContainer->setDragDropMode(QAbstractItemView::DragDrop);
	nodeContainer->setDragEnabled(true);
	nodeContainer->setMovement(QListView::Static);
	nodeContainer->setResizeMode(QListWidget::Adjust);
	nodeContainer->setDefaultDropAction(Qt::MoveAction);

	nodeContainer->setSelectionMode(QAbstractItemView::SingleSelection);
	nodeContainer->setDragEnabled(true);
	nodeContainer->setDragDropMode(QAbstractItemView::DragDrop);
	nodeContainer->viewport()->setAcceptDrops(false);
	nodeContainer->setDropIndicatorShown(true);


	//testing widget
	auto node = new nodeListModel;
	node->name = QString("test Node");
	auto item = new QListWidgetItem;
	item->setData(Qt::DisplayRole, node->name);
	item->setSizeHint(currentSize);
	item->setTextAlignment(Qt::AlignCenter);
	item->setFlags(item->flags() | Qt::ItemIsEditable);
//	item->setIcon(QIcon(":/icons/icons8-folder-72.png"));
	nodeContainer->addItem(item);
	
	//displayWidget->setWidget(sceneWidget);

}



GraphNodeScene *MainWindow::createNewScene()
{
    auto scene = new GraphNodeScene(this);
    scene->setBackgroundBrush(QBrush(QColor(60, 60, 60)));

    connect(scene, &GraphNodeScene::newConnection, [this, scene](SocketConnection* connection)
    {
        auto graph = scene->getNodeGraph();
        ShaderGenerator shaderGen;
        auto code = shaderGen.generateShader(graph);
        textEdit->setPlainText(code);
        sceneWidget->updateShader(code);
        sceneWidget->resetRenderTime();

    });

    connect(scene, &GraphNodeScene::nodeValueChanged, [this, scene](NodeModel* model, int index)
    {
        auto graph = scene->getNodeGraph();
        ShaderGenerator shaderGen;
        auto code = shaderGen.generateShader(graph);
        textEdit->setPlainText(code);
        sceneWidget->updateShader(code);
        sceneWidget->resetRenderTime();
    });

    return scene;
}
