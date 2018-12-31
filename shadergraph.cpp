#include "shadergraph.h"
#include "graph/nodegraph.h"
#include "graph/nodemodel.h"
#include "nodes/test.h"


ShaderGraph* ShaderGraph::createDefaultShaderGraph()
{
	auto nodeGraph = new ShaderGraph();
	auto masterNode = new SurfaceMasterNode();
	nodeGraph->addNode(masterNode);
	nodeGraph->setMasterNode(masterNode);

	return nodeGraph;
}

// to be implemented
ShaderGraph* ShaderGraph::createParticleShaderGraph()
{
	return nullptr;
}

// to be implemented
ShaderGraph* ShaderGraph::createPBRShaderGraph()
{
	return nullptr;
}