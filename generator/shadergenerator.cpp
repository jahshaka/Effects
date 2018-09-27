#include "shadergenerator.h"
#include "shadercontext.h"

#include "../graph/nodegraph.h"
#include "../graph/nodemodel.h"
#include "../graph/connectionmodel.h"

QString ShaderGenerator::generateShader(NodeGraph* graph)
{
	auto ctx = new ShaderContext();

	auto masterNode = graph->getMasterNode();

	generateProperties(graph, ctx);
	processNode(masterNode, ctx);

	auto code = ctx->generateUniforms();
	code += ctx->generateVars();

	code += "void surface(inout Material material){\n";
	code += ctx->generateCode(true);
	code += "}\n";

	return code;
}

void ShaderGenerator::generateProperties(NodeGraph* graph, ShaderContext* ctx)
{
	for (auto node : graph->nodes.values()) {
		for (auto sock : node->outSockets) {
			auto tempVar = ctx->createTempVar(sock);
			sock->setVarName(tempVar.name);
		}
	}
}

void ShaderGenerator::preprocess()
{

}

void ShaderGenerator::processNode(NodeModel* node, ShaderContext* ctx)
{
	for (auto sock : node->inSockets) {
		if (sock->hasConnection()) {
			auto con = sock->getConnection();
			processNode(con->leftSocket->getNode(), ctx);
		}
	}

	node->process(ctx);
}

void ShaderGenerator::postprocess()
{

}