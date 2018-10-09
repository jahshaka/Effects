#include "texturemanager.h"

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

void TextureManager::loadUnloadedTextures()
{
	for (auto tex : this->textures) {
		if (tex->dirty) {
			tex->texture = iris::Texture2D::load(tex->path);
			tex->dirty = false;
		}
	}
}

TextureManager* TextureManager::getSingleton()
{
	if (instance == nullptr)
		instance = new TextureManager();

	return instance;
}