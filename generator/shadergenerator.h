#ifndef SHADER_GENERATOR_H
#define SHADER_GENERATOR_H

#include <QString>

class ShaderContext;
class NodeModel;
class NodeGraph;

class ShaderGenerator
{
public:
	QString generateShader(NodeGraph* graph);

	void generateProperties(NodeGraph* graph, ShaderContext* ctx);

	void preprocess();

	void processNode(NodeModel* node, ShaderContext* ctx);

	void postprocess();
};

#endif// SHADER_GENERATOR_H