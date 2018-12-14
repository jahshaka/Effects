#include "materialhelper.h"
#include "../graphnodescene.h"
#include "../graph/nodegraph.h"
#include "../generator/shadergenerator.h"
#include <QjsonObject>

bool MaterialHelper::materialHasEffect(QJsonObject matObj)
{
	if (matObj.contains("shadergraph"))
		return true;
	return false;
}

QJsonObject MaterialHelper::serialize(GraphNodeScene * scene)
{
	auto graph = scene->getNodeGraph();
	QJsonObject matObj;
	matObj["name"] = graph->settings.name;
	matObj["version"] = 2.0;
	matObj["type"] = "effect";
	matObj["shaderGuid"] = "";
	// serialize properties
	//matObj["properties"] = 

	// GENERATE SHADERS
	ShaderGenerator shaderGen;
	shaderGen.generateShader(graph);
	auto vertCode = shaderGen.getVertexShader();
	auto fragCode = shaderGen.getFragmentShader();

	matObj["shadergraph"] = scene->serialize();

	matObj["vertexShaderSource"] = vertCode;
	matObj["fragmentShaderSource"] = fragCode;

	// todo: if vertex offset or alpha was connected to, generate shadow shader

	return matObj;
	//return QJsonObject();
}
