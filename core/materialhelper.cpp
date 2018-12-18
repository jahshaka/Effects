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

QString MaterialHelper::assetPath(QString relPath)
{
	//qDebug() << QDir::cleanPath(QDir::currentPath() + QDir::separator() + relPath);
	return QDir::cleanPath(QDir::currentPath() + QDir::separator() + relPath);
}

bool MaterialHelper::generateShader(NodeGraph * graph, QString & vertexShader, QString & fragmentShader)
{
	//todo: change to proper paths
	auto vertTemplate = iris::GraphicsHelper::loadAndProcessShader(assetPath("assets/surface.vert"));
	auto fragTemplate = iris::GraphicsHelper::loadAndProcessShader(assetPath("assets/surface.frag"));

	ShaderGenerator shaderGen;
	shaderGen.generateShader(graph);
	auto vertCode = shaderGen.getVertexShader();
	auto fragCode = shaderGen.getFragmentShader();

	vertexShader = vertTemplate + vertCode;
	fragmentShader = fragTemplate + fragCode;

	return true;
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
	QString vertCode, fragCode;
	generateShader(graph, vertCode, fragCode);

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

iris::CustomMaterialPtr MaterialHelper::generateMaterialFromMaterialDefinition(QJsonObject matObj, bool generateFromGraph)
{
	//auto nodeGraph = extractNodeGraphFromMaterialDefinition(matObj);

	auto shader = iris::Shader::create();

	if (generateFromGraph) {
		ShaderGenerator shaderGen;
		auto graph = extractNodeGraphFromMaterialDefinition(matObj);
		
		QString vertCode, fragCode;
		generateShader(graph, vertCode, fragCode);

		// append to shader templates
		shader->setVertexShader(vertCode);
		shader->setFragmentShader(fragCode);
	}
	else {
		shader->setVertexShader(matObj["vertexShaderSource"].toString());
		shader->setFragmentShader(matObj["fragmentShaderSource"].toString());
	}
	

	auto mat = iris::CustomMaterial::createFromShader(shader);
	mat->setMaterialDefinition(matObj);

	return mat;
}
