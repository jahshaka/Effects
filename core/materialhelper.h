#pragma once

#include <QJsonObject>
#include "irisgl/src/materials/custommaterial.h"

class GraphNodeScene;
class NodeGraph;

class MaterialHelper
{
public:
	static bool materialHasEffect(QJsonObject matObj);

	static QString assetPath(QString relPath);

	// generates vertex and fragment shader form nodegraph
	// returns true if all goes well, false if otherwose
	static bool generateShader(NodeGraph* graph, QString& vertexShader, QString& fragmentShader);
	
	// Converts a GraphNodeScene to the Material json format
	static QJsonObject serialize(GraphNodeScene* scene);
	static iris::CustomMaterialPtr createMaterialFromShaderGraph(GraphNodeScene* scene);

	static NodeGraph* extractNodeGraphFromMaterialDefinition(QJsonObject matObj);

	// uses the vertexShaderSource and fragmentShaderSource by default
	// if generateFromGraph is set to true, it generates it from the shadergraph
	// if there's an error generating the code then a null material is returned
	static iris::CustomMaterialPtr generateMaterialFromMaterialDefinition(QJsonObject matObj, bool generateFromGraph = false);
	//static QJsonObject serialize(GraphNodeScene* scene);


	static QString vertexShaderTemplate;
	static QString fragmentShaderTemplate;
};