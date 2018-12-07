#pragma once

#include <QJsonObject>

class GraphNodeScene;

class MaterialHelper
{
public:
	static bool materialHasEffect(QJsonObject matObj);
	
	static QJsonObject serialize(GraphNodeScene* scene);

	//static QJsonObject serialize(GraphNodeScene* scene);
};