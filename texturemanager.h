#pragma once

#include <QString>
#include <QMap>
#include <QVector>
#include "irisgl/include/irisgl/IrisGL.h"

class GraphTexture
{
public:
	bool dirty = true;
	iris::Texture2DPtr texture;
	QString path;
	QString uniformName;

	void setImage(QString path);
};

class TextureManager
{
public:
	QVector<GraphTexture*> textures;

	//void addTexture(QString path);
	GraphTexture* createTexture();
	void removeTexture(GraphTexture* tex);
	void loadUnloadedTextures();

	static TextureManager* getSingleton();
private:
	static TextureManager* instance;
};