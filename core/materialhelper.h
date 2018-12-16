#pragma once

#include <QJsonObject>
#include "irisgl/src/materials/custommaterial.h"

class GraphNodeScene;
class NodeGraph;

class MaterialHelper
{
public:
	static bool materialHasEffect(QJsonObject matObj);
	
	// Converts a GraphNodeScene to the Material json format
	static QJsonObject serialize(GraphNodeScene* scene);
	static iris::CustomMaterialPtr createMaterialFromShaderGraph(GraphNodeScene* scene);

	static NodeGraph* extractNodeGraphFromMaterialDefinition(QJsonObject matObj);
	//static QJsonObject serialize(GraphNodeScene* scene);
};