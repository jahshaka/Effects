#ifndef SHADER_GENERATOR_H
#define SHADER_GENERATOR_H

#include <QString>
#include <QMap>

class ShaderContext;
class NodeModel;
class NodeGraph;

struct NodePreviewShaderCache
{
	QString nodeId;
	QString code;
};

// for one-time use
class ShaderGenerator
{
public:
	QString generateShader(NodeGraph* graph);

	void generateProperties(NodeGraph* graph, ShaderContext* ctx);

	void preprocess();

	void processNode(NodeModel* node, ShaderContext* ctx);

	void postprocess();

	QMap<QString, QString> shaderPreviews;
};

#endif// SHADER_GENERATOR_H