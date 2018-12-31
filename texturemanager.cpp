#include "texturemanager.h"
#include "../mainwindow.h"

TextureManager* TextureManager::instance = 0;


void GraphTexture::setImage(QString path)
{
	this->path = path;
	this->dirty = true;
}

GraphTexture* TextureManager::createTexture()
{
	auto tex = new GraphTexture();
	tex->dirty = true;
	textures.append(tex);
	return tex;
}

void TextureManager::removeTexture(GraphTexture* tex)
{
	this->textures.removeAt(textures.indexOf(tex));
}

void TextureManager::removeTextureByGuid(QString guid)
{
	for (auto tex : textures) {
		if (tex->guid == guid) {
			// remove from list
			this->textures.removeAt(textures.indexOf(tex));
			
			// remove from db
			database->deleteAsset(guid);

			// remove from filesystem
			auto assetFolder = IrisUtils::join(
				QStandardPaths::writableLocation(QStandardPaths::DataLocation),
				"AssetStore", guid
			);

			if (QDir(assetFolder).exists()) {
				QDir(assetFolder).removeRecursively();
			}

			break;
		}
	}
}

void TextureManager::loadUnloadedTextures()
{
	for (auto tex : this->textures) {
		if (tex->dirty) {
			tex->texture = iris::Texture2D::load(tex->path);
			tex->dirty = false;
		}
	}
}

void TextureManager::setDatabase(Database * dataBase)
{
	this->database = dataBase;
}

GraphTexture * TextureManager::loadTextureFromGuid(QString guid)
{
	auto tex = this->createTexture();
	auto asset = database->fetchAsset(guid);
	if (asset.guid.isEmpty()) {
		return tex;
	}

	auto imagePath = IrisUtils::join(
		QStandardPaths::writableLocation(QStandardPaths::DataLocation),
		"AssetStore", guid, asset.name
	);
	tex->setImage(imagePath);
	tex->guid = guid;

	return tex;
}

GraphTexture* TextureManager::importTexture(QString path)
{
	auto assetPath = IrisUtils::join(
		QStandardPaths::writableLocation(QStandardPaths::DataLocation),
		"AssetStore"
	);

	auto texGuid = shadergraph::MainWindow::genGUID();

	const QString assetFolder = QDir(assetPath).filePath(texGuid);
	QDir().mkpath(assetFolder);

	QFileInfo fileInfo(path);
	QString fileToCopyTo = IrisUtils::join(assetFolder, fileInfo.fileName());
	bool copyFile = QFile::copy(fileInfo.absoluteFilePath(), fileToCopyTo);

	database->createAssetEntry(QString::null, texGuid, fileInfo.fileName(), static_cast<int>(ModelTypes::File));

	auto tex = createTexture();
	tex->path = fileToCopyTo;
	tex->guid = texGuid;

	return tex;
}

TextureManager* TextureManager::getSingleton()
{
	if (instance == nullptr)
		instance = new TextureManager();

	return instance;
}