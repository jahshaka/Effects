#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "graphnode.h"
#include <QMouseEvent>
#include <QDebug>
#include <QDrag>
#include "scenewidget.h"
#include <QLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QListWidgetItem>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QMimeData>
#include <QFile>
#include <QByteArray>
#include <QScrollBar>
//#include "graphtest.h"
#include "generator/shadergenerator.h"
#include "nodes/test.h"
#include "materialwriter.h"
//#include "nodelistitem.h"
#include "graph/library.h"
#include "nodes/libraryv1.h"
#include <QPointer>
#include "graphnodescene.h"
#include "propertywidgets/basepropertywidget.h"
#include "listwidget.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
  //  ui->setupUi(this);
	configureUI();

    scene = nullptr;
	sceneWidget = new SceneWidget();


	installEventFilter(this);

    // preview widget
 /*   sceneWidget = new SceneWidget();
    auto grid = new QGridLayout();
    grid->setSpacing(0);
    grid->setMargin(0);
    grid->addWidget(sceneWidget);
    ui->sceneContainer->setLayout(grid);*/

    // add menu items to property widget

	bar = new QMenuBar(this);
	file = new QMenu("File", bar);
	window = new QMenu("Window", bar);
	edit = new QMenu("Edit", bar);
	auto barLayout = new QVBoxLayout;
	auto actionSave = new QAction("save shader", bar);
	auto actionLoad = new QAction("load shader", bar);
	auto actionExport = new QAction("export shader", bar);
	auto actionNew = new QAction("new shader", bar);

	bar->addMenu(file);
	bar->addMenu(edit);
	bar->addMenu(window);
	bar->setLayout(barLayout);

	file->addAction(actionNew);
	file->addAction(actionSave);
	file->addAction(actionLoad);
	file->addAction(actionExport);
	file->setFont(font);

	window->addAction(nodeTray->toggleViewAction());
	window->addAction(textWidget->toggleViewAction());
	window->addAction(displayWidget->toggleViewAction());
	window->addAction(propertyWidget->toggleViewAction());
	window->addAction(materialSettingsDock->toggleViewAction());
	window->setFont(font);
	
	setMenuBar(bar);

	connect(actionSave, &QAction::triggered, this, &MainWindow::saveGraph);
	connect(actionLoad, &QAction::triggered, this, &MainWindow::loadGraph);
	connect(actionExport, &QAction::triggered, this, &MainWindow::exportGraph);
	connect(actionNew, &QAction::triggered, this, &MainWindow::newNodeGraph);

	connect(edit, &QMenu::triggered, [=]() {
	//	for (auto item : scene->nodeGraph->library->getItems()) {
	//		auto factory = item->factoryFunction;
	//		connect(menu->addAction(item->displayName), &QAction::triggered, [this, x, y, factory]() {

	//			auto node = factory();
	//			this->addNodeModel(node, x, y);

	//		});
	//	}

	//	// create properties
	//	auto propMenu = menu->addMenu("Properties");
	//	for (auto prop : nodeGraph->properties) {
	//		connect(propMenu->addAction(prop->displayName), &QAction::triggered, [this, x, y, prop]() {
	//			auto propNode = new PropertyNode();
	//			propNode->setProperty(prop);
	//			this->addNodeModel(propNode, x, y);
	//		});
	//	}
	});

	newNodeGraph();
	generateTileNode();
	configureStyleSheet();
	setMinimumSize(300, 400);
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
	sceneWidget->graphScene = newScene;
	displayWidget->setWidget(sceneWidget);
	sceneWidget->setMinimumSize(100, 100);
	materialSettingsWidget->setMaterialSettings(&graph->settings);
	this->graph = graph;
}

void MainWindow::newNodeGraph()
{
    auto graph = new NodeGraph;
	graph->setNodeLibrary(new LibraryV1());
    //registerModels(graph);
    auto masterNode = new SurfaceMasterNode();
    graph->addNode(masterNode);
    graph->setMasterNode(masterNode);

    setNodeGraph(graph);
}

MainWindow::~MainWindow()
{
    
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

    auto graph = NodeGraph::deserialize(d.object(), new LibraryV1());
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


void MainWindow::configureStyleSheet()
{
	setStyleSheet(
		"QMainWindow::separator {width: 10px;h eight: 0px; margin: -3.5px; padding: 0px; border: 0px solid black; background: rgba(19, 19, 19, 1);}"
		"QWidget{background:rgba(32,32,32,1); color:rgba(240,240,240,1); border: 0px solid rgba(0,0,0,0);}"
		"QMenu{	background: rgba(26,26,26,.9); color: rgba(250,250, 250,.9); border-radius : 2px; }"
		"QMenu::item{padding: 4px 5px 4px 10px;	}"
		"QMenu::item:hover{	background: rgba(40,128, 185,.9);}"
		"QMenu::item:selected{	background: rgba(40,128, 185,.9);}"

		"QTabWidget::pane{border: 1px solid rgba(0,0,0,.5);	border - top: 0px solid rgba(0,0,0,0);	}"
		"QTabWidget::tab - bar{	left: 1px;	}"
		"QDockWidget::tab{	background:rgba(32,32,32,1);}"

		"QScrollBar:vertical {border : 0px solid black;	background: rgba(132, 132, 132, 0);width: 24px; padding: 4px;}"
		"QScrollBar::handle{ background: rgba(72, 72, 72, 1);	border-radius: 8px; width: 14px; }"
		"QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {	background: rgba(200, 200, 200, 0);}"
		"QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical {	background: rgba(0, 0, 0, 0);border: 0px solid white;}"
		"QScrollBar::sub-line, QScrollBar::add-line {	background: rgba(10, 0, 0, .0);}"
	);

	bar->setStyleSheet(
		"QMenuBar{spacing : 5px; border: 3px solid rgba(0,0,0,.4); }"
		"QMenuBar::item:selected{ background: rgba(53,53,53,1);}"

		"QMenu{	background: rgba(26,26,26,.9); color: rgba(250,250, 250,.9);}"
		"QMenu::item{padding: 2px 5px 2px 20px;	}"
		"QMenu::item:hover{	background: rgba(40,128, 185,.9);}"
		"QMenu::item:selected{	background: rgba(40,128, 185,.9);}");

	/*nodeContainer->setStyleSheet("QListWidget::item{background: rgba(70,70,70,1); color: rgba(200,200,200,1);}"
		"QListView::item:selected:active {background: qlineargradient(x1 : 0, y1 : 0, x2 : 0, y2 : 1,stop : 0 #6a6ea9, stop: 1 #888dd9);}"
		"QWidget{background: rgba(32,32,32,1);}");*/

	propertyListWidget->setStyleSheet(
		"QWidget{background:rgba(32,32,32,1);}"
	);

	nodeContainer->setStyleSheet(
		"QListView::item{ border-radius: 2px; border: 1px solid rgba(0,0,0,1); background: rgba(80,80,80,1); margin: 3px;  }"
		"QListView::item:selected{ background: rgba(65,65,65,1); border: 1px solid rgba(50,150,250,1); }"
		"QListView::item:hover{ background: rgba(55,55,55,1); border: 1px solid rgba(50,150,250,1); }"
		"QListView::text{ top : -6; }"

	);

	nodeContainer->verticalScrollBar()->setStyleSheet(
		"QScrollBar:vertical {border : 0px solid black;	background: rgba(132, 132, 132, 0);width: 10px; }"
		"QScrollBar::handle{ background: rgba(72, 72, 72, 1);	border-radius: 5px;  left: 8px; }"
		"QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {	background: rgba(200, 200, 200, 0);}"
		"QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical {	background: rgba(0, 0, 0, 0);border: 0px solid white;}"
		"QScrollBar::sub-line, QScrollBar::add-line {	background: rgba(10, 0, 0, .0);}"
	);

	nodeTray->setStyleSheet(
		"QDockWidget{color: rgba(250,250,250,.9); background: rgba(32,32,32,1);}"
		"QDockWidget::title{ padding: 8px; background: rgba(22,22,22,1);	border: 1px solid rgba(20,20,20, .8);	text-align: center;}"
		"QDockWidget::close-button{ background: rgba(0,0,0,0); color: rgba(200,200,200,0); icon-size: 0px; padding: 23px; }"
		"QDockWidget::float-button{ background: rgba(0,0,0,0); color: rgba(200,200,200,0); icon-size: 0px; padding: 22px; }"
		//"QDockWidget::close-button, QDockWidget::float-button{	background: rgba(10,10,10,1); color: white;padding: 0px;}"
		//"QDockWidget::close-button:hover, QDockWidget::float-button:hover{background: rgba(0,220,0,0);padding: 0px;}"
		"QComboBox::drop-down {	width: 15px;  border: none; subcontrol-position: center right;}"
		"QComboBox::down-arrow{image : url(:/images/drop-down-24.png); }"
	);

	textWidget->setStyleSheet(nodeTray->styleSheet());
	displayWidget->setStyleSheet(nodeTray->styleSheet());
	propertyWidget->setStyleSheet(nodeTray->styleSheet());
	materialSettingsWidget->setStyleSheet(nodeTray->styleSheet());
	textEdit->setStyleSheet(nodeTray->styleSheet());
	materialSettingsDock->setStyleSheet(nodeTray->styleSheet());
	tabbedWidget->setStyleSheet(nodeTray->styleSheet() + 
	"QTabWidget::pane{	border: 1px solid rgba(0, 0, 0, .5); border - top: 0px solid rgba(0, 0, 0, 0);}"
	"QTabBar::tab{	background: rgba(21, 21, 21, .7); color: rgba(255, 255, 255, .9); font - weight: 400; font - size: 13em; padding: 5px 22px 5px 22px; }"
	);
	for (int i = 0; i < tabbedWidget->count(); i++) {
		tabbedWidget->widget(i)->setStyleSheet(nodeContainer->styleSheet());
	}
}

void MainWindow::configureUI()
{

	font.setPointSizeF(font.pointSize() * devicePixelRatioF());
	setFont(font);


	nodeTray = new QDockWidget("Library",this);
	centralWidget = new QWidget();
	textWidget = new QDockWidget("Code View");
	displayWidget = new QDockWidget("Display");

	propertyWidget = new QDockWidget("Properties");
	materialSettingsDock = new QDockWidget("Material Settings");
	materialSettingsWidget = new MaterialSettingsWidget;
	tabbedWidget = new QTabWidget;
	graphicsView = new GraphicsView;
	textEdit = new QTextEdit;
	propertyListWidget = new PropertyListWidget;
	nodeContainer = new QListWidget;
	splitView = new QSplitter;

	nodeTray->setAllowedAreas(Qt::AllDockWidgetAreas);
	textWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	displayWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	propertyWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	materialSettingsDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);



	setDockNestingEnabled(true);
	this->setCentralWidget(splitView);
	splitView->setOrientation(Qt::Vertical);
	splitView->addWidget(graphicsView);
	splitView->addWidget(tabbedWidget);


	//addDockWidget(Qt::LeftDockWidgetArea, nodeTray, Qt::Vertical);
	addDockWidget(Qt::LeftDockWidgetArea, textWidget, Qt::Vertical);
	addDockWidget(Qt::RightDockWidgetArea, displayWidget, Qt::Vertical);
	addDockWidget(Qt::RightDockWidgetArea, materialSettingsDock, Qt::Vertical);
	addDockWidget(Qt::LeftDockWidgetArea, propertyWidget, Qt::Vertical);
	//QMainWindow::tabifyDockWidget(propertyWidget, materialSettingsWidget);

	textWidget->setWidget(textEdit);
	propertyWidget->setWidget(propertyListWidget);
	QSize currentSize(100, 100);
	
	// main left dock widget
	auto container = new QWidget;
	auto containerLayout = new QVBoxLayout;
	container->setLayout(containerLayout);

	//search box
	auto searchContainer = new QWidget;
	auto searchLayout = new QHBoxLayout;
	auto searchBar = new QLineEdit;
	//searchContainer->setContentsMargins(0, 0, 0, 0);

	searchContainer->setLayout(searchLayout);
	searchLayout->addWidget(searchBar);
	searchLayout->addSpacing(12);

	searchBar->setPlaceholderText("search");
	searchBar->setAlignment(Qt::AlignLeft);
	searchBar->setFont(font);
	connect(searchBar, &QLineEdit::textChanged, [=](QString str) {
		nodeContainer->clear();
		QList<NodeLibraryItem*> lis;
		for (auto item : graph->library->items) {
			if (item->displayName.contains(str, Qt::CaseInsensitive)) lis.append(item);
		}
		generateTileNode(lis);
		if (str.length() == 0) {
			nodeContainer->clear();
			generateTileNode();
		}

	});

	auto assetViewToggleButtonGroup = new QButtonGroup;
	auto toggleIconView = new QPushButton(tr("Icon"));
	toggleIconView->setCheckable(true);
	toggleIconView->setCursor(Qt::PointingHandCursor);
	toggleIconView->setChecked(true);
	toggleIconView->setFont(font);

	auto toggleListView = new QPushButton(tr("List"));
	toggleListView->setCheckable(true);
	toggleListView->setCursor(Qt::PointingHandCursor);
	toggleListView->setFont(font);

	auto label = new QLabel("Display:");
	label->setFont(font);

	assetViewToggleButtonGroup->addButton(toggleIconView);
	assetViewToggleButtonGroup->addButton(toggleListView);

	QHBoxLayout *toggleLayout = new QHBoxLayout;
	toggleLayout->setSpacing(0);
	toggleLayout->addWidget(label);
	toggleLayout->addStretch();
	toggleLayout->addWidget(toggleIconView);
	toggleLayout->addWidget(toggleListView);

	connect(toggleIconView, &QPushButton::pressed, [this]() {
		nodeContainer->setViewMode(QListWidget::IconMode);
	});

	connect(toggleListView, &QPushButton::pressed, [this]() {
		nodeContainer->setViewMode(QListWidget::ListMode);
	});



	nodeTray->setWidget(container);
	materialSettingsDock->setWidget(materialSettingsWidget);

	containerLayout->addWidget(searchContainer);
	containerLayout->addWidget(nodeContainer);
	containerLayout->addSpacing(8);
	containerLayout->addLayout(toggleLayout);

	nodeContainer->setAlternatingRowColors(false);
	nodeContainer->setSpacing(0);
	nodeContainer->setContentsMargins(10, 3, 10, 10);
	nodeContainer->setViewMode(QListWidget::IconMode);
	nodeContainer->setIconSize(currentSize);
	nodeContainer->setMouseTracking(true);
	nodeContainer->setDragDropMode(QAbstractItemView::DragDrop);
	nodeContainer->setMovement(QListView::Static);
	nodeContainer->setResizeMode(QListWidget::Adjust);
	nodeContainer->setDefaultDropAction(Qt::MoveAction);
	nodeContainer->setSelectionMode(QAbstractItemView::SingleSelection);
	nodeContainer->setSelectionRectVisible(false);
	nodeContainer->setDragEnabled(true);
	nodeContainer->viewport()->setAcceptDrops(false);
	nodeContainer->setDropIndicatorShown(true);
	nodeContainer->installEventFilter(this);
	nodeContainer->viewport()->installEventFilter(this);
	nodeContainer->setWordWrap(true);
	nodeContainer->setGridSize(QSize(90, 90));
	nodeContainer->setSortingEnabled(true);
	nodeContainer->sortItems();
	nodeContainer->setEditTriggers(QAbstractItemView::NoEditTriggers);
	propertyListWidget->installEventFilter(this);

	searchContainer->setStyleSheet("background:rgba(32,32,32,1);");
	searchBar->setStyleSheet("QLineEdit{ background:rgba(41,41,41,1); border: 1px solid rgba(150,150,150,.2); border-radius: 2px; }");

	container->setStyleSheet(
		"QPushButton{ background: #333; color: #DEDEDE; border : 0; padding: 4px 16px; }"
		"QPushButton:hover{ background-color: #555; }"
		"QPushButton:pressed{ background-color: #444; }"
		"QPushButton:disabled{ color: #444; }"
		"QPushButton:checked{ background-color: rgba(50,150,255,1); }"
		"QLineEdit{ padding: 6px 10px; border-radius: 2px; }"
	);

	addTabs();
}

void MainWindow::generateTileNode()
{
	QSize currentSize(90, 90);

	for (NodeLibraryItem *tile : graph->library->items) {
		if (tile->name == "property") continue;
		auto item = new QListWidgetItem;
		item->setText(tile->displayName);
		item->setData(Qt::DisplayRole, tile->displayName);
		item->setData(Qt::UserRole, tile->name);
		item->setSizeHint(currentSize);
		item->setTextAlignment(Qt::AlignCenter);
		item->setFlags(item->flags() | Qt::ItemIsEditable);
		item->setIcon(QIcon(":/icons/icon.png"));
		item->setBackgroundColor(QColor(60, 60, 60));
		//nodeContainer->addItem(item);
		setNodeLibraryItem(item, tile);

	}
}

void MainWindow::generateTileNode(QList<NodeLibraryItem*> list)
{
	QSize currentSize(90, 90);

	for (auto tile : list) {
		
		auto item = new QListWidgetItem;
		item->setText(tile->displayName);
		item->setData(Qt::DisplayRole, tile->displayName);
		item->setData(Qt::UserRole, tile->name);
		item->setSizeHint(currentSize);
		item->setTextAlignment(Qt::AlignCenter);
		item->setFlags(item->flags() | Qt::ItemIsEditable);
		item->setIcon(QIcon(":/icons/icon.png"));
		item->setBackgroundColor(QColor(60, 60, 60));
		
		//nodeContainer->addItem(item);
		setNodeLibraryItem(item, tile);


	}
}

void MainWindow::addTabs()
{
	for (int i = 0; i < (int)NodeType::PlaceHolder; i++) {

		auto wid = new ListWidget;
		tabbedWidget->addTab(wid, NodeModel::getEnumString(static_cast<NodeType>(i)));
	}
}

void MainWindow::setNodeLibraryItem(QListWidgetItem *item, NodeLibraryItem *tile)
{
	auto wid = static_cast<QListWidget*>(tabbedWidget->widget(static_cast<int>(tile->factoryFunction()->nodeType)));
	wid->addItem(item);
	
}

bool MainWindow::eventFilter(QObject * watched, QEvent * event)
{

	if (watched == propertyListWidget) {
		
		switch (event->type()) {
			case QEvent::MouseButtonPress: {
				break;
			}

			case QEvent::MouseButtonRelease: {
				break;
			}

			case QEvent::MouseMove: {
				auto evt = static_cast<QMouseEvent*>(event);
				QPoint dragStartPosition(300, 0);
				if ((evt->pos() - dragStartPosition).manhattanLength()
					< QApplication::startDragDistance())
					return true;

				if (evt->buttons() & Qt::LeftButton) {

					auto wid = propertyListWidget->currentWidget;
					if (!wid) return true;
					if (!wid->pressed) return true;
					
					qDebug() << wid->index;
					auto drag = new QDrag(this);
					auto mimeData = new QMimeData;
					QByteArray arr;
					arr.setNum(wid->index);
					drag->setMimeData(mimeData);

					mimeData->setText(wid->modelProperty->displayName);
					mimeData->setData("index", arr);
					Qt::DropAction dropev = drag->exec(Qt::CopyAction); 
				}

				break;
			}

			default: break;
		}
	}

	return QObject::eventFilter(watched, event);
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

		// assign previews
		auto nodes = scene->getNodes();
		for (auto node : nodes) {
			if (shaderGen.shaderPreviews.contains(node->nodeId))
				node->setPreviewShader(shaderGen.shaderPreviews[node->nodeId]);
		}

    });
	
	connect(scene, &GraphNodeScene::connectionRemoved, [this, scene](SocketConnection* connection)
    {
        auto graph = scene->getNodeGraph();
        ShaderGenerator shaderGen;
        auto code = shaderGen.generateShader(graph);
        textEdit->setPlainText(code);
        sceneWidget->updateShader(code);
        sceneWidget->resetRenderTime();

		// assign previews
		auto nodes = scene->getNodes();
		for (auto node : nodes) {
			if (shaderGen.shaderPreviews.contains(node->nodeId))
				node->setPreviewShader(shaderGen.shaderPreviews[node->nodeId]);
		}
    });

    connect(scene, &GraphNodeScene::nodeValueChanged, [this, scene](NodeModel* model, int index)
    {
        auto graph = scene->getNodeGraph();
        ShaderGenerator shaderGen;
        auto code = shaderGen.generateShader(graph);
        textEdit->setPlainText(code);
        sceneWidget->updateShader(code);
        sceneWidget->resetRenderTime();

		// assign previews
		auto nodes = scene->getNodes();
		for (auto node : nodes) {
			if (shaderGen.shaderPreviews.contains(node->nodeId))
				node->setPreviewShader(shaderGen.shaderPreviews[node->nodeId]);
		}
    });

    return scene;
}
