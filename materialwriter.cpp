#include "materialwriter.h"
//#include "nodemodel.h"
//#include "graphtest.h"
#include <QJsonObject>
#include <QJsonArray>
#include "graph/nodegraph.h"
#include "generator/shadergenerator.h"

QJsonObject MaterialWriter::serializeMaterial(NodeGraph* graph)
{
	QJsonObject matObj;

	matObj["name"] =				graph->settings.name;
    matObj["type"] =				"shadergraph";
	matObj["zWrite"] =				graph->settings.zwrite;
	matObj["depthTest"] =		    graph->settings.depthTest;
	matObj["blendMode"] =			graph->settings.blendMode;
	matObj["cullMode"] =			graph->settings.cullMode;
	matObj["renderLayer"] =			graph->settings.renderLayer;
	matObj["fog"] =					graph->settings.fog;
	matObj["castShadows"] =			graph->settings.castShadow;
	matObj["receiveShadows"] =		graph->settings.receiveShadow;
	matObj["lighting"] =			graph->settings.acceptLighting;

	QJsonArray uniformArray;
	// convert properties to uniforms
	for (auto prop : graph->properties) {
		QJsonObject uniformObj;
		//uniformObj["name"] = prop->name;
		uniformObj["displayName"] = prop->displayName;
		uniformObj["name"] = prop->getUniformName();
		uniformObj["uniform"] = prop->getUniformName();

		switch (prop->type) {
		case PropertyType::Float:
			writeFloat((FloatProperty*)prop, uniformObj);
			uniformArray.append(uniformObj);
			break;
		// note: textures have a toggle value
		}
	}

	matObj["uniforms"] = uniformArray;

	// todo: add surface type for pbr
	matObj["vertex_shader"] = ":assets/shaders/surface.vert";
	matObj["fragment_shader"] = "shader.frag";
	matObj["builtin"] = false;

	// save shadergraph
	matObj["shadergraph"] = graph->serialize();
	matObj["cached_fragment_shader"] = (new ShaderGenerator())->generateShader(graph);

	return matObj;
}

void MaterialWriter::writeFloat(FloatProperty* prop, QJsonObject& uniform)
{
	uniform["value"] = prop->value;
	uniform["min"] = prop->minValue;
	uniform["max"] = prop->maxValue;
	uniform["type"] = "float";
}
