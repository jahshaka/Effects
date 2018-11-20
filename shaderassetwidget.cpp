#include "shaderassetwidget.h"
#include <QMenu>
#include <QEvent>
#include <QMouseEvent>
#include <QJsonDocument>
#include <QFileInfo>
#include <QDebug>

#include "../uimanager.h"
#include "../globals.h"

#include "../core/database/database.h"
//#include "../widgets/assetwidget.h"

#include "../core/guidmanager.h"
//#include "../../irisgl/src/core/irisutils.h"
//#include "../io/assetmanager.h"




ShaderAssetWidget::ShaderAssetWidget(Database *handle) : QWidget()
{

	layout = new QVBoxLayout;
	setLayout(layout);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	breadCrumbs = new QHBoxLayout;
	auto breadCrumbsWidget = new QWidget;
	breadCrumbsWidget->setLayout(breadCrumbs);

	assetViewWidget = new ListWidget;
	assetViewWidget->viewport()->installEventFilter(this);

	layout->addWidget(breadCrumbsWidget);
	

	qDebug() << UiManager::isSceneOpen;

		if (UiManager::isSceneOpen) {
			qDebug() << "scene is open"; 
		}
		else {


			noWidget = new QWidget;
			auto noWidgetLayout = new QVBoxLayout;
			auto lableIcon = new QLabel;
			auto lableText = new QLabel;
			noWidget->setLayout(noWidgetLayout);
			noWidgetLayout->addWidget(lableIcon);
			noWidgetLayout->addWidget(lableText);
			lableText->setText("No Open projects");
			lableIcon->setPixmap(QPixmap(":/icons/icons8-empty-box-50.png"));
			lableIcon->setAlignment(Qt::AlignCenter);
			lableText->setAlignment(Qt::AlignCenter);
			noWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

			layout->addWidget(noWidget);
		}
	
}


ShaderAssetWidget::~ShaderAssetWidget()
{
	
}

void ShaderAssetWidget::createCustomContextMenu(const QPoint pos)
{

	QMenu menu;
	menu.setStyleSheet(
		"QMenu { background-color: #1A1A1A; color: #EEE; padding: 0; margin: 0; }"
		"QMenu::item { background-color: #1A1A1A; padding: 6px 8px; margin: 0; }"
		"QMenu::item:selected { background-color: #3498db; color: #EEE; padding: 6px 8px; margin: 0; }"
		"QMenu::item : disabled { color: #555; }"
	);

	QAction *action;

	QMenu *createMenu = menu.addMenu("Create");
	action = new QAction(QIcon(), "Shader", this);
	connect(action, SIGNAL(triggered()), this, SLOT(createShader()));
	createMenu->addAction(action);

	action = new QAction(QIcon(), "New Folder", this);
	connect(action, SIGNAL(triggered()), this, SLOT(createFolder()));
	createMenu->addAction(action);

	action = new QAction(QIcon(), "Import Asset", this);
	connect(action, SIGNAL(triggered()), this, SLOT(importAsset()));
	menu.addAction(action);

	//action = new QAction(QIcon(), "Delete", this);
	//connect(action, SIGNAL(triggered()), this, SLOT(deleteTreeFolder()));
	//menu.addAction(action);

	menu.exec(assetViewWidget->mapToGlobal(pos));
}

void ShaderAssetWidget::updateAssetView(const QString & path, int filter, bool showDependencies)
{
	assetViewWidget->clear();

	if (filter > 0) {
		for (const auto &asset : db->fetchChildAssets(path, filter, showDependencies)) addItem(asset);
	}
	else {
		for (const auto &folder : db->fetchChildFolders(path)) addItem(folder);
		for (const auto &asset : db->fetchChildAssets(path, filter, showDependencies)) addItem(asset);  /* TODO : irk this out */
		//addCrumbs(db->fetchCrumbTrail(path));
	}

	//goUpOneControl->setEnabled(false);

}

void ShaderAssetWidget::addItem(const FolderRecord & folderData)
{
	if (!folderData.visible) return;

	QListWidgetItem *item = new QListWidgetItem;
	item->setData(Qt::DisplayRole, folderData.name);
	item->setData(MODEL_ITEM_TYPE, MODEL_FOLDER);
	item->setData(MODEL_GUID_ROLE, folderData.guid);
	item->setData(MODEL_PARENT_ROLE, folderData.parent);

	item->setSizeHint(currentSize);
	item->setTextAlignment(Qt::AlignCenter);
	item->setFlags(item->flags() | Qt::ItemIsEditable);
	item->setIcon(QIcon(":/icons/icons8-folder-72.png"));

	assetViewWidget->addItem(item);
}

void ShaderAssetWidget::addItem(const AssetRecord & assetData)
{
	auto prop = QJsonDocument::fromBinaryData(assetData.properties).object();
	if (!prop["type"].toString().isEmpty()) {
		// No need to check further, this is a builtin asset
		return;
	}

	QListWidgetItem *item = new QListWidgetItem;
	item->setData(Qt::DisplayRole, QFileInfo(assetData.name).baseName());
	item->setData(Qt::UserRole, assetData.name);
	item->setData(MODEL_TYPE_ROLE, assetData.type);
	item->setData(MODEL_ITEM_TYPE, MODEL_ASSET);
	item->setData(MODEL_GUID_ROLE, assetData.guid);
	item->setData(MODEL_PARENT_ROLE, assetData.parent);

	QPixmap thumbnail;
	if (thumbnail.loadFromData(assetData.thumbnail, "PNG")) {
		item->setIcon(QIcon(thumbnail));
	}
	else {
		item->setIcon(QIcon(":/icons/empty_object.png"));
	}

	if (assetData.type == static_cast<int>(ModelTypes::Texture)) {

	}

	if (assetData.type == static_cast<int>(ModelTypes::Shader)) {
		item->setData(MODEL_TYPE_ROLE, assetData.type);
		item->setIcon(QIcon(":/icons/icons8-file-72.png"));
	}

	if (assetData.type == static_cast<int>(ModelTypes::ParticleSystem)) {
		item->setData(MODEL_TYPE_ROLE, assetData.type);
		item->setIcon(QIcon(":/icons/icons8-file-72-ps.png"));
	}

	if (assetData.type == static_cast<int>(ModelTypes::File)) {
		item->setData(MODEL_TYPE_ROLE, assetData.type);
		// TODO - make this some generic value all assets can use
		//item->setData(MODEL_MESH_ROLE, shaderAssetName.name);
		item->setIcon(QIcon(":/icons/icons8-file-72-file.png"));
	}

	if (assetData.type == static_cast<int>(ModelTypes::Material)) {
		item->setData(MODEL_TYPE_ROLE, assetData.type);
	}

	if (assetData.type == static_cast<int>(ModelTypes::Object)) {
		const QString meshAssetGuid =
			db->getDependencyByType(static_cast<int>(ModelTypes::Mesh), assetData.guid);
		item->setData(MODEL_TYPE_ROLE, assetData.type);
		item->setData(MODEL_MESH_ROLE, db->fetchAsset(meshAssetGuid).name);
	}

	item->setSizeHint(currentSize);
	item->setTextAlignment(Qt::AlignCenter);
	item->setFlags(item->flags() | Qt::ItemIsEditable);

	// Hide meshes for now, we work with objects which are parents for meshes, materials etc
	assetViewWidget->addItem(item);
}

void ShaderAssetWidget::setUpDatabse(Database * db)
{
	//remove noWidget if preset and add assetViewWidget
	qDebug() << UiManager::isSceneOpen;
	if (UiManager::isSceneOpen) {

		if (noWidget) {
			layout->removeWidget(noWidget);
			layout->addWidget(assetViewWidget);
		}

		this->db = db;
		updateAssetView(Globals::project->getProjectGuid());
	}
	else {
		layout->addWidget(noWidget);
		
	}
}

void ShaderAssetWidget::refresh()
{
	updateAssetView(Globals::project->getProjectGuid());
}

void ShaderAssetWidget::createFolder()
{
	const QString newFolder = "New Folder";
	QListWidgetItem *item = new QListWidgetItem;
	item->setFlags(item->flags() | Qt::ItemIsEditable);
	item->setSizeHint(currentSize);
	item->setTextAlignment(Qt::AlignCenter);
	item->setIcon(QIcon(":/icons/icons8-folder-72.png"));

	item->setData(MODEL_GUID_ROLE, GUIDManager::generateGUID());
	item->setData(MODEL_PARENT_ROLE, assetItemShader.selectedGuid);
	item->setData(MODEL_ITEM_TYPE, MODEL_FOLDER);

	assetItemShader.wItem = item;

	QString folderName = newFolder;
	QStringList foldersInProject = db->fetchFolderNameByParent(assetItemShader.selectedGuid);

	// If we encounter the same file, make a duplicate...
	int increment = 1;
	while (foldersInProject.contains(folderName)) {
		folderName = newFolder + " " + QString::number(increment++);
	}

	const QString guid = item->data(MODEL_GUID_ROLE).toString();
	const QString parent = item->data(MODEL_PARENT_ROLE).toString();

	//// Create a new database entry for the new folder
	db->createFolder(folderName, parent, guid);

	// We could just addItem but this is by choice and also so we can order folders first
	updateAssetView(assetItemShader.selectedGuid);
}

//void ShaderAssetWidget::createShader()
//{
//	const QString newShader = "Untitled Shader";
//	QListWidgetItem *item = new QListWidgetItem;
//	item->setFlags(item->flags() | Qt::ItemIsEditable);
//	item->setSizeHint(currentSize);
//	item->setTextAlignment(Qt::AlignCenter);
//	item->setIcon(QIcon(":/icons/icons8-file-72.png"));
//
//	const QString assetGuid = GUIDManager::generateGUID();
//
//	item->setData(MODEL_GUID_ROLE, assetGuid);
//	item->setData(MODEL_PARENT_ROLE, assetItem.selectedGuid);
//	item->setData(MODEL_ITEM_TYPE, MODEL_ASSET);
//	item->setData(MODEL_TYPE_ROLE, static_cast<int>(ModelTypes::Shader));
//
//	assetItem.wItem = item;
//
//	QString shaderName = newShader;
//
//	QStringList assetsInProject = db->fetchAssetNameByParent(assetItem.selectedGuid);
//
//	//// If we encounter the same file, make a duplicate...
//	int increment = 1;
//	while (assetsInProject.contains(IrisUtils::buildFileName(shaderName, "shader"))) {
//		shaderName = QString(newShader + " %1").arg(QString::number(increment++));
//	}
//
//	db->createAssetEntry(assetGuid,
//		IrisUtils::buildFileName(shaderName, "shader"),
//		static_cast<int>(ModelTypes::Shader),
//		assetItem.selectedGuid,
//		QByteArray());
//
//	item->setText(shaderName);
//	assetViewWidget->addItem(item);
//
//	QFile *templateShaderFile = new QFile(IrisUtils::getAbsoluteAssetPath("app/templates/ShaderTemplate.shader"));
//	templateShaderFile->open(QIODevice::ReadOnly | QIODevice::Text);
//	QJsonObject shaderDefinition = QJsonDocument::fromJson(templateShaderFile->readAll()).object();
//	templateShaderFile->close();
//	shaderDefinition["name"] = shaderName;
//	shaderDefinition.insert("guid", assetGuid);
//
//	auto assetShader = new AssetShader;
//	assetShader->fileName = IrisUtils::buildFileName(shaderName, "shader");
//	assetShader->assetGuid = assetGuid;
//	//assetShader->path = IrisUtils::join(Globals::project->getProjectFolder(), IrisUtils::buildFileName(shaderName, "shader"));
//	assetShader->setValue(QVariant::fromValue(shaderDefinition));
//
//	db->updateAssetAsset(assetGuid, QJsonDocument(shaderDefinition).toBinaryData());
//
//	AssetManager::addAsset(assetShader);
//}

bool ShaderAssetWidget::eventFilter(QObject * watched, QEvent * event)
{

	if (watched == assetViewWidget->viewport()) {
		switch (event->type())
		{

		case QEvent::MouseButtonPress: {
			auto evt = static_cast<QMouseEvent*>(event);
			if (evt->button() == Qt::RightButton) {
				QModelIndex index = assetViewWidget->indexAt(evt->pos());
				if (index.isValid()) {

				}
				else {
					createCustomContextMenu(evt->pos());
				}
			}
		}
			
		default:
			break;
		}
	}
	return QObject::eventFilter(watched, event);

}
