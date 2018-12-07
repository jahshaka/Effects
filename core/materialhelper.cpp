#include "materialhelper.h"
#include "../graphnodescene.h"
#include "../graph/nodegraph.h"
#include <QjsonObject>

bool MaterialHelper::materialHasEffect(QJsonObject matObj)
{
	if (matObj.contains("shadergraph"))
		return true;
	return false;
}

QJsonObject MaterialHelper::serialize(GraphNodeScene * scene)
{
	auto graph = scene->nodeGraph;
	QJsonObject matObj;
	matObj["name"] = graph->settings.name;
	matObj["version"] = 2.0;
	matObj["shaderGuid"] = "";
	// serialize properties
	//matObj["properties"] = 

	return QJsonObject();
}
