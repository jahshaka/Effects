#include "materialhelper.h"
#include "../graphnodescene.h"
#include "../graph/nodegraph.h"
#include "../generator/shadergenerator.h"
#include <QjsonObject>
#include "irisgl/src/materials/custommaterial.h"
#include "irisgl/src/graphics/shader.h"
#include "../nodes/libraryv1.h"
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>

/*
EFFECT MATERIAL FORMAT
===============
{
	version:2,
	name:"",
	type:"shader", //shader, fx, postprocess
	shaderGuid:"", // or

	vertexShaderSource:"",
	fragmentShaderSource:"",

	// if custom shadows
	shadowVertexShaderSource:"",
	shadowFragmentShaderSource:"",

	// effect
	shadergraph:{

	},

	values : [

	],

	properties : {

	},

	states : {

	}
}


*/

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

QJsonObject MaterialHelper::serialize(NodeGraph* graph)
{
	QJsonObject matObj;
	matObj["name"] = graph->settings.name;
	matObj["version"] = 2.0;
	matObj["type"] = "effect";
	matObj["shaderGuid"] = "";


	// GENERATE SHADERS
	QString vertCode, fragCode;
	generateShader(graph, vertCode, fragCode);
	matObj["shadergraph"] = graph->serialize();

	matObj["vertexShaderSource"] = vertCode;
	matObj["fragmentShaderSource"] = fragCode;

	// todo: if vertex offset or alpha was connected to, generate shadow shader

	// properties are the same as the ones in the shadergraph, they're
	// just placed here for convenience
	matObj["properties"] = matObj["shadergraph"].toObject()["properties"];

	return matObj;
}

iris::CustomMaterialPtr MaterialHelper::createMaterialFromShaderGraph(NodeGraph* graph)
{
	auto matObj = serialize(graph);

	auto shader = iris::Shader::create();
	shader->setVertexShader(matObj["vertexShaderSource"].toString());
	shader->setFragmentShader(matObj["fragmentShaderSource"].toString());

	auto mat = iris::CustomMaterial::createFromShader(shader);
	mat->setMaterialDefinition(matObj);

	return mat;
}

NodeGraph* MaterialHelper::extractNodeGraphFromMaterialDefinition(QJsonObject matObj)
{
	auto graphObj = matObj["shadergraph"].toObject();
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
		auto vertShader = matObj["vertexShaderSource"].toString();
		auto fragShader = matObj["fragmentShaderSource"].toString();
		shader->setVertexShader(vertShader);
		shader->setFragmentShader(fragShader);
	}
	

	auto mat = iris::CustomMaterial::createFromShader(shader);
	mat->setMaterialDefinition(matObj);
	mat->setVersion(2);

	// generate properties
	parseMaterialProperties(mat, matObj["properties"].toArray());

	return mat;
}

void MaterialHelper::parseMaterialProperties(iris::CustomMaterialPtr material, QJsonArray propList)
{
	for (int i = 0; i < propList.size(); i++) {
		auto prop = propList[i].toObject();
		auto displayName = prop["displayName"].toString();
		auto name = prop["name"].toString();
		auto uniform = prop["uniform"].toString();

		if (prop["type"] == "float") {
			auto fltProp = new iris::FloatProperty;
			fltProp->id = i;
			fltProp->displayName = displayName;
			fltProp->name = name;
			fltProp->minValue = prop["min"].toDouble();
			fltProp->maxValue = prop["max"].toDouble();
			fltProp->uniform = uniform;
			fltProp->value = prop["value"].toDouble();

			material->properties.append(fltProp);
		}

		if (prop["type"] == "int") {
			auto fltProp = new iris::IntProperty;
			fltProp->id = i;
			fltProp->displayName = displayName;
			fltProp->name = name;
			fltProp->minValue = prop["min"].toDouble();
			fltProp->maxValue = prop["max"].toDouble();
			fltProp->uniform = uniform;
			fltProp->value = prop["value"].toDouble();

			material->properties.append(fltProp);
		}

		if (prop["type"] == "bool") {
			auto blProp = new iris::BoolProperty;
			blProp->id = i;
			blProp->displayName = displayName;
			blProp->name = name;
			blProp->uniform = uniform;
			blProp->value = prop["value"].toBool();

			material->properties.append(blProp);
		}

		if (prop["type"] == "texture") {
			auto texProp = new iris::TextureProperty;
			texProp->id = i;
			texProp->displayName = displayName;
			texProp->name = name;
			texProp->uniform = uniform;
			texProp->toggleValue = prop["toggle"].toString();
			texProp->value = prop["value"].toString();

			material->properties.append(texProp);
		}

		if (prop["type"] == "color") {
			auto clrProp = new iris::ColorProperty;
			clrProp->id = i;
			clrProp->displayName = displayName;
			clrProp->name = name;
			clrProp->uniform = uniform;

			QColor col;
			col.setNamedColor(prop["value"].toString());
			clrProp->value = col;

			material->properties.append(clrProp);
		}

		if (prop["type"] == "vec2") {
			auto vecProp = new iris::Vec2Property;
			vecProp->id = i;
			vecProp->displayName = displayName;
			vecProp->name = name;
			vecProp->uniform = uniform;

			auto valObj = prop["value"].toObject();
			vecProp->value = QVector2D(valObj["x"].toDouble(), valObj["y"].toDouble());

			material->properties.append(vecProp);
		}

		if (prop["type"] == "vec3") {
			auto vecProp = new iris::Vec3Property;
			vecProp->id = i;
			vecProp->displayName = displayName;
			vecProp->name = name;
			vecProp->uniform = uniform;

			auto valObj = prop["value"].toObject();
			vecProp->value = QVector3D(valObj["x"].toDouble(), valObj["y"].toDouble(), valObj["z"].toDouble());

			material->properties.append(vecProp);
		}

		if (prop["type"] == "vec4") {
			auto vecProp = new iris::Vec4Property;
			vecProp->id = i;
			vecProp->displayName = displayName;
			vecProp->name = name;
			vecProp->uniform = uniform;

			auto valObj = prop["value"].toObject();
			vecProp->value = QVector4D(valObj["x"].toDouble(), valObj["y"].toDouble(), valObj["z"].toDouble(), valObj["w"].toDouble());

			material->properties.append(vecProp);
		}
	}
}
