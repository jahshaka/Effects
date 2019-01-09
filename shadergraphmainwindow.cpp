﻿#include "mainwindow.h"
//#include "ui_mainwindow.h"
#include "graphnode.h"
#include <QMouseEvent>
#include <QApplication>
#include <QButtonGroup>
#include <QDebug>
#include <QDrag>
#include "scenewidget.h"
#include <QLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QListWidgetItem>
#include <QMenuBar>
#include <QMenu>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QMimeData>
#include <QFile>
#include <QByteArray>
#include <QScrollBar>
#include <QShortcut>
#include "generator/shadergenerator.h"
#include "nodes/test.h"
#include "materialwriter.h"
#include "core/materialhelper.h"
#include "graph/library.h"
#include "nodes/libraryv1.h"
#include <QPointer>
#include "graphnodescene.h"
#include "propertywidgets/basepropertywidget.h"
#include "dialogs/searchdialog.h"
#include "listwidget.h"
#include "scenewidget.h"
#include "core/project.h"
#include "propertywidgets/texturepropertywidget.h"

#include <QMainWindow>
#include <QStandardPaths>
#include <QDirIterator>
#include <QMessageBox>

#if(EFFECT_BUILD_AS_LIB)
#include "../core/database/database.h"
#include "../uimanager.h"
#include "../globals.h"
#include "../core/guidmanager.h"
#include "../../irisgl/src/core/irisutils.h"
#include "../io/assetmanager.h"
#else
#include <QUuid>
#endif

#include "texturemanager.h"
#include <QDebug>
namespace shadergraph
{

MainWindow::MainWindow( QWidget *parent, Database *database) :
    QMainWindow(parent)
{
  //  ui->setupUi(this); 
	scene = nullptr;
	sceneWidget = new SceneWidget();
	fontIcons = new QtAwesome;
	fontIcons->initFontAwesome();
	configureUI();
	configureToolbar();
	addMenuToSceneWidget();

	installEventFilter(this);

	if (database) {
		setAssetWidgetDatabase(database);
		TextureManager::getSingleton()->setDatabase(database);
	}

	newNodeGraph();
	generateTileNode();
	configureStyleSheet();
	configureProjectDock();
	configureAssetsDock();
    configureConnections();
	setMinimumSize(300, 400);
    loadShadersFromDisk();
	
}

void MainWindow::setNodeGraph(NodeGraph *graph)
{
    // create and set new scene
    auto newScene = createNewScene();
	graphicsView->setScene(newScene);
	graphicsView->setAcceptDrops(true);
    newScene->setNodeGraph(graph);

    // delet old scene and reassign new scene
    if (scene) {
        scene->deleteLater();
    }
    scene = newScene;



	propertyListWidget->setNodeGraph(graph);
	sceneWidget->setNodeGraph(graph);
	sceneWidget->graphScene = newScene;
	materialSettingsWidget->setMaterialSettings(&graph->settings);
	sceneWidget->setMaterialSettings(graph->settings);
	this->graph = graph;
}

void MainWindow::newNodeGraph(QString *shaderName, int *templateType, QString *templateName)
{
    auto graph = new NodeGraph;
	graph->setNodeLibrary(new LibraryV1());
    auto masterNode = new SurfaceMasterNode();
    graph->addNode(masterNode);
    graph->setMasterNode(masterNode);
    setNodeGraph(graph);
}

void MainWindow::refreshShaderGraph()
{
#if(EFFECT_BUILD_AS_LIB)
	assetWidget->refresh();
#endif
	setCurrentShaderItem();
}

MainWindow::~MainWindow()
{
    
}

void MainWindow::saveShader()
{
	if (currentShaderInformation.GUID == "") {
		saveDefaultShader();
		return;
	}

	QJsonDocument doc;
	auto matObj = MaterialHelper::serialize(graph);
	doc.setObject(matObj);
	QString data = doc.toJson();

#if(EFFECT_BUILD_AS_LIB)
	dataBase->updateAssetAsset(currentShaderInformation.GUID, doc.toBinaryData());
#else

	auto filePath = QDir().filePath(QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/Materials/MyFx/");
	if (!QDir(filePath).exists()) QDir().mkpath(filePath);
	auto shaderFile = new QFile(filePath + obj["name"].toString());
	if (shaderFile->open(QIODevice::ReadWrite)) {
		shaderFile->write(doc.toJson());
		shaderFile->close();
	}
	else {
		qDebug() << "device not open";
	}
#endif
}

void MainWindow::saveDefaultShader()
{
	bool shouldSaveGraph = createNewGraph(false);
	if(shouldSaveGraph) saveShader();
}

void MainWindow::loadShadersFromDisk()
{
	// create constants for this
	auto filePath = QDir().filePath(QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/Materials/MyFx/");
	QDirIterator it(filePath);

	while (it.hasNext()) {

		QFile file(it.next());
		file.open(QIODevice::ReadOnly);
		auto doc = QJsonDocument::fromJson(file.readAll());
		file.close();

		auto obj = doc.object();
        if (obj["guid"].toString() == "") continue;

		QListWidgetItem *item = new QListWidgetItem;
		item->setFlags(item->flags() | Qt::ItemIsEditable);
		item->setSizeHint(defaultItemSize);
		item->setTextAlignment(Qt::AlignCenter);
		item->setIcon(QIcon(":/icons/icons8-file-72.png"));

		item->setData(Qt::DisplayRole, obj["name"].toString());
		item->setData(MODEL_GUID_ROLE, obj["guid"].toString());
		item->setData(MODEL_TYPE_ROLE, static_cast<int>(ModelTypes::Shader));
		effects->addItem(item);
    }
	
}

void MainWindow::saveMaterialFile(QString filename, TexturePropertyWidget* widget)
{
	
#if(EFFECT_BUILD_AS_LIB)
	TextureManager::getSingleton()->removeTextureByGuid(widget->getValue());
	auto tex = TextureManager::getSingleton()->importTexture(filename);
	widget->setValue(tex->guid);

#else

	auto filePath = QDir().filePath(QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/Materials/Textures/");
	if (!QDir(filePath).exists()) QDir().mkpath(filePath);
	auto shaderFile = new QFile(filePath + guid);
	if (shaderFile->open(QIODevice::ReadWrite)) {
		shaderFile->write(doc.toJson());
		shaderFile->close();
	}

#endif
}

void MainWindow::deleteMaterialFile(QString filename)
{
#if(EFFECT_BUILD_AS_LIB)

    QJsonDocument doc;
    doc.setObject(graph->serialize());

#endif
}

QString MainWindow::genGUID()
{
	auto id = QUuid::createUuid();
	auto guid = id.toString().remove(0, 1);
	guid.chop(1);
	return guid;
}

void MainWindow::importGraph()
{
    QString path = QFileDialog::getOpenFileName(this, "Choose file name","material.json","Material File (*.json)");
	importGraphFromFilePath(path);
}

NodeGraph* MainWindow::importGraphFromFilePath(QString filePath, bool assign)
{
	QFile file(filePath);
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	auto val = file.readAll();
	file.close();
	QJsonDocument d = QJsonDocument::fromJson(val);

	auto obj = d.object();
	auto graph = MaterialHelper::extractNodeGraphFromMaterialDefinition(obj);

	if (assign) {
		this->setNodeGraph(graph);
		regenerateShader();
	}
	
	return graph;
}

void MainWindow::loadGraph(QString guid)
{
    propertyListWidget->clearPropertyList();
	NodeGraph *graph;

#if(EFFECT_BUILD_AS_LIB)
	QJsonObject obj = QJsonDocument::fromBinaryData(fetchAsset(guid)).object();

	graph = MaterialHelper::extractNodeGraphFromMaterialDefinition(obj);
	this->setNodeGraph(graph);
	//this->restoreGraphPositions(obj["shadergraph"].toObject());
#else
	auto filePath = QDir().filePath(QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/Materials/MyFx/");
	QDirIterator it(filePath);
	QJsonObject obj;

	while (it.hasNext()) {

		QFile file(it.next());
		file.open(QIODevice::ReadOnly);
		auto doc = QJsonDocument::fromJson(file.readAll());
		file.close();

		auto obj1 = doc.object();
        if (obj1["guid"].toString() == guid) {
			obj = obj1;
			break;
		}
	}
	graph = NodeGraph::deserialize(obj["graph"].toObject(), new LibraryV1());
	this->setNodeGraph(graph);
	this->restoreGraphPositions(obj["graph"].toObject());
#endif

	
	regenerateShader();
	currentProjectShader = selectCorrectItemFromDrop(guid);
	currentShaderInformation.GUID = currentProjectShader->data(MODEL_GUID_ROLE).toString();
	oldName = currentShaderInformation.name = currentProjectShader->data(Qt::DisplayRole).toString(); 
	restoreGraphPositions(obj["shadergraph"].toObject());
}

void MainWindow::exportGraph()
{
	QString path = QFileDialog::getSaveFileName(this, "Choose file name", "effect.effect", "Material File (*.effect)");

	QJsonDocument doc;
	doc.setObject((new MaterialWriter())->serializeMaterial(graph));

	QFile file(path);
	file.open(QFile::WriteOnly | QFile::Truncate);
	file.write(doc.toJson());
	file.close();

	QString sourcePath = QFileInfo(path).absolutePath()+"/shader.frag";
	QFile sourceFile(sourcePath);
	sourceFile.open(QFile::WriteOnly | QFile::Truncate);
	ShaderGenerator gen;
	gen.generateShader(graph);
	sourceFile.write("#pragma include <surface.frag>\n\n" + (gen.getFragmentShader().toUtf8()));
	//sourceFile.write("#pragma include <surface.frag>\n\n"+(new ShaderGenerator())->generateShader(graph).toUtf8());
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

bool MainWindow::deleteShader(QString guid)
{

    auto item = selectCorrectItemFromDrop(guid);
    auto holder = item->listWidget();

#if(EFFECT_BUILD_AS_LIB)

    if(dataBase->deleteAsset(guid)){
        holder->takeItem(holder->row(item));
        currentShaderInformation = shaderInfo();
        return true;
    }
#else

    auto filePath = QDir().filePath(QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/Materials/MyFx/");
    QDirIterator it(filePath);

    while (it.hasNext()) {

        QFile file(it.next());
        file.open(QIODevice::ReadOnly);
        auto doc = QJsonDocument::fromJson(file.readAll());
        file.close();

        auto obj = doc.object();
        if (obj["guid"].toString() == "") continue;
        if(obj["guid"].toString() == guid){
            if(file.remove()){
                holder->takeItem(holder->row(item));
                currentShaderInformation = shaderInfo();
                return true;
            }
        }

    }

#endif
    return false;

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

	propertyListWidget->setStyleSheet(
		"QWidget{background:rgba(32,32,32,1);}"
	);

	nodeContainer->setStyleSheet(
		"QListView::item{ border-radius: 2px; border: 1px solid rgba(0,0,0,.31); background: rgba(51,51,51,1); margin: 3px;  }"
		"QListView::item:selected{ background: rgba(155,155,155,1); border: 1px solid rgba(50,150,250,.1); }"
		"QListView::item:hover{ background: rgba(95,95,95,1); border: .1px solid rgba(50,150,250,.1); }"
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
	"QTabBar::tab{	background: rgba(21, 21, 21, .7); color: rgba(250, 250, 250, .9); font - weight: 400; font - size: 13em; padding: 5px 22px 5px 22px; }"
		"QTabBar::tab:selected{ color: rgba(255, 255, 255, .99); border-top: 2px solid rgba(50,150,250,.8); }"
		"QTabBar::tab:!selected{ background: rgba(55, 55, 55, .99); border : 1px solid rgba(21,21,21,.4); color: rgba(200,200,200,.5); }"
	);
	for (int i = 0; i < tabbedWidget->count(); i++) {
		tabbedWidget->widget(i)->setStyleSheet(nodeContainer->styleSheet());
	}
}


void MainWindow::configureProjectDock()
{
#if(EFFECT_BUILD_AS_LIB)
	auto widget = new QWidget;
	auto layout = new QVBoxLayout;
	widget->setLayout(layout);
	layout->setContentsMargins(0, 0, 0, 0);
	projectDock->setWidget(widget);
	projectDock->setStyleSheet(nodeTray->styleSheet());

	auto searchContainer = new QWidget;
	auto searchLayout = new QHBoxLayout;
	auto searchBar = new QLineEdit;

	searchContainer->setLayout(searchLayout);
	searchLayout->addWidget(searchBar);
	searchLayout->addSpacing(12);

	searchBar->setPlaceholderText("search");
	searchBar->setAlignment(Qt::AlignLeft);
	searchBar->setFont(font);
	searchBar->setTextMargins(8, 0, 0, 0);
	searchBar->setStyleSheet("QLineEdit{ background:rgba(41,41,41,1); border: 1px solid rgba(150,150,150,.2); border-radius: 1px; color: rgba(250,250,250,.95); }");


	layout->addWidget(assetWidget);
#endif
}


void MainWindow::configureAssetsDock()
{

	auto holder = new QWidget;
	auto layout = new QVBoxLayout;
	holder->setLayout(layout);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);

	auto tabWidget = new QTabWidget;
	presets = new ListWidget;
	effects = new ListWidget;
	effects->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    effects->shaderContextMenuAllowed = true;
    //tabWidget->addTab(presets, "Presets");
	//tabWidget->addTab(effects, "My Fx");
	//tabWidget->setStyleSheet(tabbedWidget->styleSheet());

	auto scrollView = new QScrollArea;
	auto contentHolder = new QWidget;
	auto contentLayout = new QVBoxLayout;
	contentHolder->setLayout(contentLayout);
	scrollView->setWidget(contentHolder);
	scrollView->setWidgetResizable(true);
	scrollView->setContentsMargins(0, 0, 0, 0);
	scrollView->setStyleSheet(
		"QScrollBar:vertical {border : 0px solid black;	background: rgba(132, 132, 132, 0);width: 10px; }"
		"QScrollBar::handle{ background: rgba(72, 72, 72, 1);	border-radius: 3px;  left: 8px; }"
		"QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {	background: rgba(200, 200, 200, 0);}"
		"QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical {	background: rgba(0, 0, 0, 0);border: 0px solid white;}"
		"QScrollBar::sub-line, QScrollBar::add-line {	background: rgba(10, 0, 0, .0);}"
	);

	auto presetsLabel = new QLabel("Presets");
	auto effectsLabel = new QLabel("My Fx");

	presetsLabel->setStyleSheet("QLabel{ background: rgba(20,20,20,1); padding: 3px; padding-left: 8px; color: rgba(200,200,200,1); }");
	effectsLabel->setStyleSheet(presetsLabel->styleSheet());

	contentLayout->addWidget(presetsLabel);
	contentLayout->addWidget(presets);
	contentLayout->addWidget(effectsLabel);
	contentLayout->addWidget(effects);
	contentLayout->setContentsMargins(0, 0, 0, 0);

	presets->setStyleSheet(presets->styleSheet() +
		"border: 1px solid black;"
	);

	// get list of presets
	for (auto tile : CreateNewDialog::getPresetList()) {
		auto item = new QListWidgetItem;
		item->setText(tile.name);
		item->setSizeHint(defaultItemSize);
		item->setTextAlignment(Qt::AlignBottom);
		presets->addToListWidget(item);
	}

	presets->isResizable = true;
	effects->isResizable = true;

	scrollView->adjustSize();
	
	auto buttonBar = new QWidget;
	auto buttonLayout = new QHBoxLayout;
	auto exportBtn = new QPushButton("help");
	auto importBtn = new QPushButton("help");
	auto addBtn = new QPushButton("+");
	{
		int fontSize = 12;

		buttonBar->setLayout(buttonLayout);
		buttonLayout->addWidget(exportBtn);
		buttonLayout->addWidget(importBtn);
		buttonLayout->addWidget(addBtn);
		buttonLayout->setContentsMargins(2, 2, 2, 2);
		buttonLayout->setSpacing(1);


		exportBtn->setText(QChar(fa::upload));
		exportBtn->setFont(fontIcons->font(fontSize));
		exportBtn->setToolTip("Export shader");
		importBtn->setText(QChar(fa::download));
		importBtn->setFont(fontIcons->font(fontSize));
		importBtn->setToolTip("Import shader");
		addBtn->setText(QChar(fa::plus));
		addBtn->setFont(fontIcons->font(fontSize));
		addBtn->setToolTip("Create new shader");

		exportBtn->setCursor(Qt::PointingHandCursor);
		importBtn->setCursor(Qt::PointingHandCursor);
		addBtn->setCursor(Qt::PointingHandCursor);

		exportBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		importBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		addBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		buttonBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
		presetsLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
		effectsLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

		exportBtn->setStyleSheet(
			"QPushButton{background: rgba(51,51,51,1); color:rgba(230,230,230,1); border: 1px solid rgba(50,50,50,.1); padding: 5px 10px; }"
			"QPushButton:hover{background: rgba(100,100,100,.3); color:rgba(230,230,230,1);}"
		);
		importBtn->setStyleSheet(exportBtn->styleSheet());
		addBtn->setStyleSheet(exportBtn->styleSheet());

		buttonBar->setStyleSheet(
			"background: rgba(21,21,21,1); padding :0px;"
		);
		buttonBar->setContentsMargins(0, 0, 0, 0);

		connect(exportBtn, &QPushButton::clicked, [=]() {
			exportGraph();
		});
		connect(importBtn, &QPushButton::clicked, [=]() {
			importGraph();
		});
		connect(addBtn, &QPushButton::clicked, [=]() {
			createNewGraph();
		});
	}

	layout->addWidget(scrollView);
	layout->addWidget(buttonBar);
	assetsDock->setWidget(holder);
	assetsDock->setStyleSheet(nodeTray->styleSheet());

	updateAssetDock();
}

void MainWindow::createShader(NodeGraphPreset preset, bool loadNewGraph)
{
	QString newShader;
	newShader = preset.title;

	QListWidgetItem *item = new QListWidgetItem;
	item->setFlags(item->flags() | Qt::ItemIsEditable);
	item->setSizeHint(defaultItemSize);
	item->setTextAlignment(Qt::AlignCenter);
	item->setIcon(QIcon(":/icons/icons8-file-72.png"));

	auto assetGuid = genGUID();

	item->setData(MODEL_GUID_ROLE, assetGuid);
	item->setData(MODEL_ITEM_TYPE, MODEL_ASSET);
	item->setData(MODEL_TYPE_ROLE, static_cast<int>(ModelTypes::Shader));
	item->setData(Qt::DisplayRole, newShader);

	currentProjectShader = item;
	oldName = newShader;

	currentShaderInformation.GUID = assetGuid;
	currentShaderInformation.name = newShader;

	//QStringList assetsInProject = dataBase->fetchAssetNameByParent(assetItemShader.selectedGuid);

	//// If we encounter the same file, make a duplicate...
	int increment = 1;
	//while (assetsInProject.contains(IrisUtils::buildFileName(shaderName, "shader"))) {
	//	shaderName = QString(newShader + " %1").arg(QString::number(increment++));
	//}

	item->setText(newShader);
	effects->addItem(item);
	effects->displayAllContents();

	propertyListWidget->clearPropertyList();
    auto graph = importGraphFromFilePath(MaterialHelper::assetPath(preset.templatePath), false);
	int i = 0;
	for (auto prop : graph->properties) {
		if (prop->type == PropertyType::Texture) {
			auto graphTexture = TextureManager::getSingleton()->importTexture(preset.list.at(i));
			prop->setValue(graphTexture->guid);
			i++;
		}
	}
	setNodeGraph(graph);
	regenerateShader();


#if(EFFECT_BUILD_AS_LIB)

	auto shaderDefinition = MaterialHelper::serialize(graph);
	dataBase->createAssetEntry(QString::null, assetGuid,newShader,static_cast<int>(ModelTypes::Shader), QJsonDocument(shaderDefinition).toBinaryData());
	auto assetShader = new AssetMaterial;
	assetShader->fileName = newShader;
	assetShader->assetGuid = assetGuid;
	assetShader->path = IrisUtils::join(Globals::project->getProjectFolder(), IrisUtils::buildFileName(newShader, "shader"));
	assetShader->setValue(QVariant::fromValue(MaterialHelper::createMaterialFromShaderGraph(graph)));
	dataBase->updateAssetAsset(assetGuid, QJsonDocument(shaderDefinition).toBinaryData());
	AssetManager::addAsset(assetShader);
#endif
	
	saveShader();
}

void MainWindow::setCurrentShaderItem()
{
 	if (scene->currentlyEditing)
		currentProjectShader = selectCorrectItemFromDrop(scene->currentlyEditing->data(MODEL_GUID_ROLE).toString());
}

QByteArray MainWindow::fetchAsset(QString string)
{
#if(EFFECT_BUILD_AS_LIB)
	return dataBase->fetchAssetData(string);
#else
	// fetch file locally

#endif



	return QByteArray();
}

void MainWindow::configureUI()
{
	font.setPointSizeF(font.pointSize() * devicePixelRatioF());
//	setFont(font);

	nodeTray = new QDockWidget("Library");
	centralWidget = new QWidget();
	textWidget = new QDockWidget("Code View");
	displayWidget = new QDockWidget("Display");
	assetsDock = new QDockWidget("Assets");
	projectDock = new QDockWidget("Project Assets");

	propertyWidget = new QDockWidget("Properties");
	materialSettingsDock = new QDockWidget("Material Settings");
	materialSettingsWidget = new MaterialSettingsWidget;
	tabbedWidget = new QTabWidget;
	graphicsView = new GraphicsView;
	textEdit = new QTextEdit;
	propertyListWidget = new PropertyListWidget;
	nodeContainer = new QListWidget;
	splitView = new QSplitter;
	projectName = new QLineEdit;

	nodeTray->setAllowedAreas(Qt::AllDockWidgetAreas);
	textWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	displayWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	propertyWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	materialSettingsDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	projectDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	assetsDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

	setDockNestingEnabled(true);
	this->setCentralWidget(splitView);
	splitView->setOrientation(Qt::Vertical);
	splitView->addWidget(graphicsView);
	splitView->addWidget(tabbedWidget);
	splitView->setStretchFactor(0, 90);

#if(EFFECT_BUILD_AS_LIB)
	assetWidget = new ShaderAssetWidget;
	addDockWidget(Qt::LeftDockWidgetArea, projectDock, Qt::Vertical);
#endif
	addDockWidget(Qt::LeftDockWidgetArea, assetsDock, Qt::Vertical);
//	addDockWidget(Qt::RightDockWidgetArea, textWidget, Qt::Vertical);
	addDockWidget(Qt::RightDockWidgetArea, displayWidget, Qt::Vertical);
	addDockWidget(Qt::LeftDockWidgetArea, materialSettingsDock, Qt::Vertical);
	addDockWidget(Qt::RightDockWidgetArea, propertyWidget, Qt::Vertical);

	//auto displayLayout = new QVBoxLayout;
	//displayWidget->setLayout(displayLayout);
	//displayWidget->setWidget(sceneWidget);
	displayWidget->setMinimumSize(300, 230);

	textWidget->setWidget(textEdit);
	propertyWidget->setWidget(propertyListWidget);
	propertyListWidget->setMinimumHeight(400);
	
	QSize currentSize(100, 100);

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

	connect(propertyListWidget, &PropertyListWidget::nameChanged, [=](QString name, QString id) {
		scene->updatePropertyNodeTitle(name, id);
	});

	connect(propertyListWidget, &PropertyListWidget::texturePicked, [=](QString fileName, TexturePropertyWidget* widget) {
		saveMaterialFile(fileName, widget);
	});

	connect(propertyListWidget, &PropertyListWidget::imageRequestedForTexture, [=](QString guid) {
		auto assetPath = IrisUtils::join(
			QStandardPaths::writableLocation(QStandardPaths::DataLocation),
			"AssetStore"
		);
		QString assetFolder = QDir(assetPath).filePath(guid);

	});

	connect(propertyListWidget, &PropertyListWidget::deleteProperty, [=](QString propID) {
		qDebug() << propID;
		graph->deletePropertyById(propID);
	});

	materialSettingsDock->setWidget(materialSettingsWidget);

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
	nodeContainer->setGridSize(defaultGridSize);
	nodeContainer->setSortingEnabled(true);
	nodeContainer->sortItems();
	nodeContainer->setEditTriggers(QAbstractItemView::NoEditTriggers);
	propertyListWidget->installEventFilter(this);

	addTabs();
	
}

void MainWindow::configureToolbar()
{
	QVariantMap options;
	options.insert("color", QColor(255, 255, 255));
	options.insert("color-active", QColor(255, 255, 255));

	toolBar = new QToolBar("Tool Bar");
	toolBar->setIconSize(QSize(15, 15));	

	QFont projectNameFont = projectName->font();
	projectNameFont.setPixelSize(24);
	projectNameFont.setWeight(65);
	projectName->setFont(font);
	

	projectName->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
	projectName->setMinimumWidth(250);
	projectName->setText("Untitled Shader");
	projectName->setStyleSheet(
		"QLineEdit{background: rgba(0,0,0,0); border-radius: 3px; padding-left: 5px; color: rgba(255,255,255,.8); }"
		"QLineEdit:hover{ background : rgba(21,21,21,1); color: rgba(255,255,255,1);}"
	);

	connect(projectName, &QLineEdit::textEdited, [=](const QString text) {

		currentProjectShader->setData(Qt::DisplayRole, text);
		currentProjectShader->setData(Qt::UserRole, text);
		newName = text;
	});

	connect(projectName, &QLineEdit::editingFinished, [=]() {
		//update finished, update view;
		saveShader();
		renameShader();
	});


    //toolBar->addWidget(projectName);

	QAction *actionUndo = new QAction;
	actionUndo->setToolTip("Undo | Undo last action");
	actionUndo->setObjectName(QStringLiteral("actionUndo"));
	actionUndo->setIcon(fontIcons->icon(fa::reply, options));
	toolBar->addAction(actionUndo);

	QAction *actionRedo = new QAction;
	actionRedo->setToolTip("Redo | Redo last action");
	actionRedo->setObjectName(QStringLiteral("actionRedo"));
	actionRedo->setIcon(fontIcons->icon(fa::share, options));
	toolBar->addAction(actionRedo);

	toolBar->addSeparator();

	auto exportBtn = new QAction;
	auto importBtn = new QAction;
	auto addBtn = new QAction;

	exportBtn->setIcon(fontIcons->icon(fa::upload, options));
	exportBtn->setToolTip("Export shader");

	importBtn->setIcon(fontIcons->icon(fa::download, options));
	importBtn->setToolTip("Import shader");

	addBtn->setIcon(fontIcons->icon(fa::plus, options));
	addBtn->setToolTip("Create new shader");

	toolBar->addActions({ exportBtn, importBtn, addBtn });




	// this acts as a spacer
	QWidget* empty = new QWidget();
	empty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	toolBar->addWidget(empty);

	QAction *actionSave = new QAction;
	actionSave->setObjectName(QStringLiteral("actionSave"));
	actionSave->setCheckable(false);
	actionSave->setToolTip("Export | Export the current scene");
	actionSave->setIcon(fontIcons->icon(fa::floppyo, options));
	toolBar->addAction(actionSave);

	this->addToolBar(toolBar);

	connect(actionSave, &QAction::triggered, this, &MainWindow::saveShader);
	connect(exportBtn, &QAction::triggered, this, &MainWindow::exportGraph);
	connect(importBtn, &QAction::triggered, this, &MainWindow::importGraph);
	connect(addBtn, &QAction::triggered, this, &MainWindow::createNewGraph);

	toolBar->setStyleSheet(""
		//"QToolBar{background: rgba(48,48,48, 1); border: .5px solid rgba(20,20,20, .8); border-bottom: 1px solid rgba(20,20,20, .8); padding: 0px;}"
		"QToolBar{ background: rgba(48,48,48,1); border-bottom: 1px solid rgba(20,20,20, .8);}"
		"QToolBar::handle:horizontal { image: url(:/icons/thandleh.png); width: 24px; }"
		//"QToolBar::handle:vertical { image: url(:/icons/thandlev.png); height: 22px;}"
		"QToolBar::separator { background: rgba(0,0,0,.2); width: 1px; height : 20px;}"
		"QToolBar::separator:horizontal { background: #272727; width: 1px; margin-left: 6px; margin-right: 6px;} "
		"QToolButton { border-radius: 2px; background: rgba(33,33,33, 1); color: rgba(250,250,250, 1); border : 1px solid rgba(10,10,10, .4); font: 18px; padding: 8px; } "
		"QToolButton:hover{ background: rgba(48,48,48, 1); } "
	//	"QToolButton:checked{ background: rgba(50,150,250,1); }"
	);

	empty->setStyleSheet(
		"background : rgba(0,0,0,0);"
	);
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
		item->setSizeHint(defaultItemSize);
		item->setTextAlignment(Qt::AlignCenter);
		item->setFlags(item->flags() | Qt::ItemIsEditable);
		item->setIcon(QIcon(":/icons/icon.png"));
		item->setBackgroundColor(QColor(60, 60, 60));
		item->setData(MODEL_TYPE_ROLE, QString("node"));
		setNodeLibraryItem(item, tile);

	}
}


// never used, consider deleting
void MainWindow::generateTileNode(QList<NodeLibraryItem*> list)
{
	QSize currentSize(90, 90);

	for (auto tile : list) {
		
		auto item = new QListWidgetItem;
		item->setText(tile->displayName);
		item->setData(Qt::DisplayRole, tile->displayName);
		item->setData(Qt::UserRole, tile->name);
		item->setSizeHint(defaultItemSize);
		item->setTextAlignment(Qt::AlignCenter);
		item->setFlags(item->flags() | Qt::ItemIsEditable);
		item->setIcon(QIcon(":/icons/icon.png"));
		item->setBackgroundColor(QColor(60, 60, 60));
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

bool MainWindow::createNewGraph(bool loadNewGraph)
{
	//TODO get presets from database
	list.clear();

	if (loadNewGraph) {
			
	}
	CreateNewDialog node;
	node.exec();

	if (node.result() == QDialog::Accepted) {
		auto preset = node.getPreset();
		createShader(preset, loadNewGraph);
		return true;
	}
	return false;
}

void MainWindow::updateAssetDock()
{

#if(EFFECT_BUILD_AS_LIB)
		for (const auto &asset : dataBase->fetchAssets())  //dp something{
		{
			if (asset.projectGuid == "" && asset.type == static_cast<int>(ModelTypes::Shader)) {
				 
				auto item = new QListWidgetItem;
				item->setText(asset.name);
				item->setFlags(item->flags() | Qt::ItemIsEditable);
				item->setSizeHint(defaultItemSize);
				item->setIcon(QIcon(":/icons/icons8-file-72.png"));
				item->setTextAlignment( Qt::AlignHCenter | Qt::AlignBottom);

				item->setData(Qt::UserRole, asset.name);
				item->setData(Qt::DisplayRole, asset.name);
				item->setData(MODEL_GUID_ROLE, asset.guid);
				item->setData(MODEL_TYPE_ROLE, asset.type);
				effects->addToListWidget(item);
			}
		}
#endif

}

void MainWindow::setAssetWidgetDatabase(Database * db)
{
#if(EFFECT_BUILD_AS_LIB)
	TextureManager::getSingleton()->setDatabase(db);
    assetWidget->setUpDatabase(db);
#endif
}

void MainWindow::renameShader()
{
#if(EFFECT_BUILD_AS_LIB)
	dataBase->renameAsset(currentProjectShader->data(MODEL_GUID_ROLE).toString(), currentProjectShader->data(Qt::DisplayRole).toString());
#else
	auto filePath = QDir().filePath(QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/Materials/MyFx/");
	if (!QDir(filePath).exists()) return;
	auto shaderFileOld = new QFile(filePath + oldName);
	auto shaderFileNew = new QFile(filePath + newName);
	QDir().rename(shaderFileOld->fileName() , shaderFileNew->fileName());
	
#endif
	oldName = currentProjectShader->data(Qt::DisplayRole).toString();
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
		shaderGen.generateShader(graph);
		auto code = shaderGen.getFragmentShader();

        textEdit->setPlainText(code);
		sceneWidget->setVertexShader(shaderGen.getVertexShader());
		sceneWidget->setFragmentShader(shaderGen.getFragmentShader());
		sceneWidget->updateShader();
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
		shaderGen.generateShader(graph);
		auto code = shaderGen.getFragmentShader();

        textEdit->setPlainText(code);
		sceneWidget->setVertexShader(shaderGen.getVertexShader());
		sceneWidget->setFragmentShader(shaderGen.getFragmentShader());
		sceneWidget->updateShader();
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
		shaderGen.generateShader(graph);
		auto code = shaderGen.getFragmentShader();

        textEdit->setPlainText(code);
		sceneWidget->setVertexShader(shaderGen.getVertexShader());
		sceneWidget->setFragmentShader(shaderGen.getFragmentShader());
		sceneWidget->updateShader();
        sceneWidget->resetRenderTime();

		// assign previews
		auto nodes = scene->getNodes();
		for (auto node : nodes) {
			if (shaderGen.shaderPreviews.contains(node->nodeId))
				node->setPreviewShader(shaderGen.shaderPreviews[node->nodeId]);
		}
    });

	connect(scene, &GraphNodeScene::loadGraph, [=](QListWidgetItem *item) {
		currentShaderInformation.name = item->data(Qt::DisplayRole).toString();
		currentShaderInformation.GUID = item->data(MODEL_GUID_ROLE).toString();
		loadGraph(currentShaderInformation.GUID);
	});

    return scene;
}

void MainWindow::regenerateShader()
{
	ShaderGenerator shaderGen;
	shaderGen.generateShader(graph);
	auto code = shaderGen.getFragmentShader();

	textEdit->setPlainText(code);
	sceneWidget->setVertexShader(shaderGen.getVertexShader());
	sceneWidget->setFragmentShader(shaderGen.getFragmentShader());
	sceneWidget->updateShader();
	sceneWidget->resetRenderTime();

	// assign previews
	auto nodes = scene->getNodes();
	for (auto node : nodes) {
		if (shaderGen.shaderPreviews.contains(node->nodeId))
			node->setPreviewShader(shaderGen.shaderPreviews[node->nodeId]);
	}
}

QListWidgetItem * MainWindow::selectCorrectItemFromDrop(QString guid)
{

	for (int i = 0; i < effects->count(); i++)
	{
		if (guid == effects->item(i)->data(MODEL_GUID_ROLE)) {
			return effects->item(i);
		}
	}

#if(EFFECT_BUILD_AS_LIB)
	for (int i = 0; i < assetWidget->assetViewWidget->count(); i++)
	{
		if (guid == assetWidget->assetViewWidget->item(i)->data(MODEL_GUID_ROLE)) {
			return assetWidget->assetViewWidget->item(i);
		}
	}
#endif


    return nullptr;
}

void MainWindow::configureConnections()
{
    connect(effects, &QListWidget::itemDoubleClicked, [=](QListWidgetItem *item) {
        currentShaderInformation.name = item->data(Qt::DisplayRole).toString();
        currentShaderInformation.GUID = item->data(MODEL_GUID_ROLE).toString();
        loadGraph(currentShaderInformation.GUID);
    });

    connect(effects, &QListWidget::itemPressed, [=](QListWidgetItem *item){
        pressedShaderInfo.name = item->data(Qt::DisplayRole).toString();
        pressedShaderInfo.GUID = item->data(MODEL_GUID_ROLE).toString();
    });



    QShortcut *shortcut = new QShortcut(QKeySequence("f"), this);
    connect(shortcut, &QShortcut::activated, [=]() {
        auto dialog = new SearchDialog(this->graph);
        dialog->exec();
    });

    //connections for MyFx sections

    connect(effects, &ListWidget::renameShader, [=](QString guid){
        auto item = selectCorrectItemFromDrop(guid);
        effects->editItem(item);
    });
    connect(effects, &ListWidget::exportShader, [=](QString guid){
        exportGraph();
    });
    connect(effects, &ListWidget::editShader, [=](QString guid){
        loadGraph(guid);
    });
    connect(effects, &ListWidget::deleteShader, [=](QString guid){
        deleteShader(guid);
    });
    connect(effects, &ListWidget::createShader, [=](QString guid){
        createNewGraph();
    });
    connect(effects, &ListWidget::importShader, [=](QString guid){

    });


    // change: any settings changed
    connect(materialSettingsWidget, &MaterialSettingsWidget::settingsChanged,[=](MaterialSettings settings){
		graph->settings = settings;
    });

    //connection for renaming item
    connect(effects->itemDelegate(), &QAbstractItemDelegate::commitData,[=](){
        //item finished editing
        editingFinishedOnListItem();
    });

}

void MainWindow::editingFinishedOnListItem()
{
    QListWidgetItem *item = selectCorrectItemFromDrop(pressedShaderInfo.GUID);
    auto oldName = pressedShaderInfo.name;
    auto newName = item->data(Qt::DisplayRole).toString();

	if (oldName == newName) return;

#if(EFFECT_BUILD_AS_LIB)
    QJsonDocument doc;
    QJsonObject obj = QJsonDocument::fromBinaryData(fetchAsset(pressedShaderInfo.GUID)).object();
    auto graph = MaterialHelper::extractNodeGraphFromMaterialDefinition(obj);
    graph->settings.name = newName;
    auto go = graph->serialize();

    auto shadergraph = obj["shadergraph"].toObject();
    auto graphObj = shadergraph["graph"].toObject();
    auto settings = graphObj["settings"].toObject();
    settings["name"] = newName;

    graphObj["settings"] = settings;
    shadergraph["graph"] = graphObj;
    obj["shadergraph"] = shadergraph;

    doc.setObject(obj);
    dataBase->updateAssetAsset(pressedShaderInfo.GUID,doc.toBinaryData());
    dataBase->renameAsset(pressedShaderInfo.GUID, newName);
#else
    // get json obj from file and edit graph like above

    auto filePath = QDir().filePath(QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/Materials/MyFx/");
    if (!QDir(filePath).exists()) return;
    auto shaderFileOld = new QFile(filePath + oldName);
    auto shaderFileNew = new QFile(filePath + newName);
    QDir().rename(shaderFileOld->fileName() , shaderFileNew->fileName());
#endif

	item->setData(Qt::DisplayRole, newName);

    // update current settings if the same
    if(pressedShaderInfo.GUID == currentShaderInformation.GUID){
        currentShaderInformation.name = newName;
        this->graph->settings.name = newName;
        materialSettingsWidget->setName(newName);
        saveShader();
    }

	pressedShaderInfo = shaderInfo();
}

void MainWindow::addMenuToSceneWidget()
{
	QMenu *modelMenu = new QMenu("model");
	QMenu *sceneMenu = new QMenu("scene");
	QMenu *backgroundMenu = new QMenu("background");
	modelMenu->setStyleSheet(
		"QMenu { background-color: #1A1A1A; color: #EEE; padding: 0; margin: 0; }"
		"QMenu:hover { background-color: #3498db; }"
		"QMenu::item { background-color: #1A1A1A; padding: 6px 16px; margin: 0; }"
		"QMenu::item:selected { background-color: #3498db; color: #EEE; }"
		"QMenu::item : disabled { color: #555; }"
	);
	sceneMenu->setStyleSheet(modelMenu->styleSheet());
	backgroundMenu->setStyleSheet(modelMenu->styleSheet());

	QMainWindow *window = new QMainWindow;
	QToolBar *bar = new QToolBar;

	window->menuBar()->addMenu(modelMenu);
	window->menuBar()->addMenu(sceneMenu);
	window->menuBar()->addMenu(backgroundMenu);
	displayWidget->setWidget(window);
	window->setCentralWidget(sceneWidget);
	
	auto cubeAction = new QAction("cube");
	connect(cubeAction, &QAction::triggered, [=]() {});		
	auto planeAction = new QAction("plane");
	connect(planeAction, &QAction::triggered, [=]() {});
	auto sphareAction = new QAction("sphere");
	connect(sphareAction, &QAction::triggered, [=]() {});
	auto customModelAction = new QAction("custom");
	connect(customModelAction, &QAction::triggered, [=]() {});
	modelMenu->addActions({cubeAction,planeAction,sphareAction,customModelAction});

	auto whiteAction = new QAction("white");
	connect(whiteAction, &QAction::triggered, [=]() {});
	auto blackAction = new QAction("black");
	connect(blackAction, &QAction::triggered, [=]() {});
	auto checkeredAction = new QAction("checkered");
	connect(checkeredAction, &QAction::triggered, [=]() {});
	auto blankColorAction = new QAction("blank");
	connect(blankColorAction, &QAction::triggered, [=]() {});
	sceneMenu->addActions({ whiteAction,blackAction,checkeredAction,blankColorAction });

	auto blankAction = new QAction("blank");
	connect(blankAction, &QAction::triggered, [=]() {});
	auto gradientAction = new QAction("custom");
	connect(gradientAction, &QAction::triggered, [=]() {});
	backgroundMenu->addActions({ blankAction,gradientAction });

	cubeAction->setCheckable(true);
	planeAction->setCheckable(true);
	sphareAction->setCheckable(true);
	customModelAction->setCheckable(true);
	whiteAction->setCheckable(true);
	blackAction->setCheckable(true);
	checkeredAction->setCheckable(true);
	blankAction->setCheckable(true);
	blankColorAction->setCheckable(true);
	gradientAction->setCheckable(true);

	sphareAction->setChecked(true);
	whiteAction->setChecked(true);
	blankAction->setChecked(true);

	auto screenShotBtn = new QPushButton("screenshot");
	bar->addWidget(screenShotBtn);
	connect(screenShotBtn, &QPushButton::clicked, [=]() {
		
	});



}

}