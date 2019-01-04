#include "shaderassetwidget.h"
#include <QMenu>
#include <QEvent>
#include <QMouseEvent>
#include <QJsonDocument>
#include <QFileInfo>
#include <QDebug>


#include "irisgl/src/core/irisutils.h"
#include "core/materialhelper.h"


#if(EFFECT_BUILD_AS_LIB)
#include "../io/assetmanager.h"
#include "../uimanager.h"
#include "../globals.h"
#include "../core/database/database.h"
#include "../core/guidmanager.h"
#endif

ShaderAssetWidget::ShaderAssetWidget(Database *handle) : QWidget()
{

	layout = new QVBoxLayout;
	setLayout(layout);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	breadCrumbs = new QHBoxLayout;
	auto breadCrumbsWidget = new QWidget;
	breadCrumbsWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	breadCrumbsWidget->setLayout(breadCrumbs);

	assetViewWidget = new ShaderListWidget;
    assetViewWidget->shaderContextMenuAllowed = true;
	assetViewWidget->setGridSize({ 95,95 });
	connect(assetViewWidget, &ShaderListWidget::itemDropped, [=](QListWidgetItem *item) {
		createShader(item);
	});

    if(handle) setUpDatabase(handle);

	stackWidget = new QStackedWidget;

	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(breadCrumbsWidget);
	layout->addWidget(stackWidget);

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

	closeBtn = new QPushButton("Hide Panel");
	noWidgetLayout->addWidget(closeBtn);

	stackWidget->addWidget(assetViewWidget);
	stackWidget->addWidget(noWidget);
	this->setWidgetToBeShown();
	
}


ShaderAssetWidget::~ShaderAssetWidget()
{
	
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

	setWidgetToBeShown();

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

	auto doc = QJsonDocument::fromBinaryData(assetData.asset);
	auto obj = doc.object();
	auto name = obj["name"].toString();

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

void ShaderAssetWidget::setUpDatabase(Database * db)
{
	//remove noWidget if preset and add assetViewWidget
	if (UiManager::isSceneOpen) {
		this->db = db;
		updateAssetView(Globals::project->getProjectGuid());
		assetItemShader.selectedGuid = Globals::project->getProjectGuid();
		
	}
}

void ShaderAssetWidget::refresh()
{
	updateAssetView(Globals::project->getProjectGuid());
}

void ShaderAssetWidget::setWidgetToBeShown()
{
	if (UiManager::isSceneOpen) {
		stackWidget->setCurrentIndex(0);
	}
	else {
		stackWidget->setCurrentIndex(1);

	}
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

void ShaderAssetWidget::createShader(QString *shaderName)
{
	QString newShader;
	if (shaderName)	 newShader = *shaderName;
	else   newShader = "Untitled Material";
	QListWidgetItem *item = new QListWidgetItem;
	item->setFlags(item->flags() | Qt::ItemIsEditable);
	item->setSizeHint(currentSize);
	item->setTextAlignment(Qt::AlignCenter);
	item->setIcon(QIcon(":/icons/icons8-file-72.png"));

	const QString assetGuid = GUIDManager::generateGUID();

	item->setData(MODEL_GUID_ROLE, assetGuid);
	item->setData(MODEL_PARENT_ROLE, assetItemShader.selectedGuid);
	item->setData(MODEL_ITEM_TYPE, MODEL_ASSET);
	item->setData(MODEL_TYPE_ROLE, static_cast<int>(ModelTypes::Material));
	assetItemShader.wItem = item;


	QStringList assetsInProject = db->fetchAssetNameByParent(assetItemShader.selectedGuid);

	//// If we encounter the same file, make a duplicate...
	int increment = 1;
	while (assetsInProject.contains(IrisUtils::buildFileName(newShader, "shader"))) {
		newShader = QString(newShader + " %1").arg(QString::number(increment++));
	}

	db->createAssetEntry(assetGuid,
		IrisUtils::buildFileName(newShader, "shader"),
		static_cast<int>(ModelTypes::Material),
		assetItemShader.selectedGuid,
		QByteArray());

	item->setText(newShader);
	assetViewWidget->addItem(item);

	QFile *templateShaderFile = new QFile(IrisUtils::getAbsoluteAssetPath("app/templates/ShaderTemplate.shader"));
	templateShaderFile->open(QIODevice::ReadOnly | QIODevice::Text);
	QJsonObject shaderDefinition = QJsonDocument::fromJson(templateShaderFile->readAll()).object();
	templateShaderFile->close();
	shaderDefinition["name"] = newShader;
	shaderDefinition.insert("guid", assetGuid);

	auto assetShader = new AssetMaterial;
	assetShader->fileName = IrisUtils::buildFileName(newShader, "material");
	assetShader->assetGuid = assetGuid;
	assetShader->path = IrisUtils::join(Globals::project->getProjectFolder(), IrisUtils::buildFileName(newShader, "shader"));
	assetShader->setValue(QVariant::fromValue(shaderDefinition));

	db->updateAssetAsset(assetGuid, QJsonDocument(shaderDefinition).toBinaryData());

	AssetManager::addAsset(assetShader);
}

void ShaderAssetWidget::createShader(QListWidgetItem * item)
{
	const QString newShader = "Untitled Shader";
	
	item->setSizeHint(currentSize);
	
	auto genGuid = []() {
		auto id = QUuid::createUuid();
		auto guid = id.toString().remove(0, 1);
		guid.chop(1);
		return guid;
	};

	const QString targetGuid = genGuid();

	
	assetItemShader.wItem = item;

	QString shaderName = item->data(Qt::DisplayRole).toString();
	QString sourceGuid = item->data(MODEL_GUID_ROLE).toString();

	AssetRecord sourceRecord = db->fetchAsset(sourceGuid);
	auto sourceData = db->fetchAssetData(sourceGuid);

	db->createAssetEntry(targetGuid,
		sourceRecord.name,
		static_cast<int>(ModelTypes::Material),
		assetItemShader.selectedGuid);
	db->updateAssetAsset(targetGuid, sourceData);

	// todo: create new item
	assetViewWidget->addItem(item);

	auto assetShader = new AssetMaterial;
	assetShader->fileName = IrisUtils::buildFileName(shaderName, "material");
	assetShader->assetGuid = targetGuid;
	assetShader->path = IrisUtils::join(Globals::project->getProjectFolder(), IrisUtils::buildFileName(shaderName, "shader"));

	//auto assetData = db->fetchAssetData(targetGuid);
	auto matObj = QJsonDocument::fromBinaryData(sourceData).object();
	auto mat = MaterialHelper::generateMaterialFromMaterialDefinition(matObj, true);
	assetShader->setValue(QVariant::fromValue(mat));

	//db->updateAssetAsset(assetGuid, QJsonDocument::fromBinaryData(fetchAsset(item->data(MODEL_GUID_ROLE).toString())).toBinaryData());

	// build material from definition
	AssetManager::addAsset(assetShader);
}

QByteArray ShaderAssetWidget::fetchAsset(QString string)
{
	return db->fetchAssetData(string);
}


