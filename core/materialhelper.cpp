#include "materialhelper.h"
#include "../graphnodescene.h"
#include "../graph/nodegraph.h"
#include "../generator/shadergenerator.h"
#include <QjsonObject>
#include "irisgl/src/materials/custommaterial.h"
#include "irisgl/src/graphics/shader.h"
#include "../nodes/libraryv1.h"

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

iris::CustomMaterialPtr MaterialHelper::createMaterialFromShaderGraph(GraphNodeScene* scene)
{
	auto matObj = serialize(scene);

	auto shader = iris::Shader::create();
	shader->setVertexShader(matObj["vertexShaderSource"].toString());
	shader->setFragmentShader(matObj["fragmentShaderSource"].toString());

	auto mat = iris::CustomMaterial::createFromShader(shader);
	mat->setMaterialDefinition(matObj);

	return mat;
}

NodeGraph* MaterialHelper::extractNodeGraphFromMaterialDefinition(QJsonObject matObj)
{
	auto graphObj = matObj["shadergraph"].toObject()["graph"].toObject();
	auto graph = NodeGraph::deserialize(graphObj, new LibraryV1());

	return graph;
}