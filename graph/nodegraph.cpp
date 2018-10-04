#include "nodegraph.h"
#include "connectionmodel.h"
#include "../nodes/test.h"
#include "library.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

void NodeGraph::addProperty(Property* prop)
{
	this->properties.append(prop);
}

Property *NodeGraph::getPropertyByName(const QString &name)
{
	for (auto prop : properties)
		if (prop->name == name)
			return prop;
	return nullptr;
}

Property *NodeGraph::getPropertyById(const QString &id)
{
	for (auto prop : properties)
		if (prop->id == id)
			return prop;
	return nullptr;
}

/*
void NodeGraph::registerModel(QString name, std::function<NodeModel *()> factoryFunction)
{
	modelFactories.insert(name, factoryFunction);
}
*/

void NodeGraph::setNodeLibrary(NodeLibrary* lib)
{
	this->library = lib;
}


void NodeGraph::addNode(NodeModel *model)
{
	nodes.insert(model->id, model);
}

void NodeGraph::setMasterNode(NodeModel *masterNode)
{
	this->masterNode = masterNode;
}

NodeModel *NodeGraph::getMasterNode()
{
	return masterNode;
}

ConnectionModel* NodeGraph::addConnection(NodeModel *leftNode, int leftSockIndex, NodeModel *rightNode, int rightSockIndex)
{
	// todo: check if the indices are correct
	return addConnection(leftNode->id, leftSockIndex, rightNode->id, rightSockIndex);
}

ConnectionModel* NodeGraph::addConnection(QString leftNodeId, int leftSockIndex, QString rightNodeId, int rightSockIndex)
{
	// todo: check if the ids and socket indices are correct
	auto leftNode = nodes[leftNodeId];
	auto leftSock = leftNode->outSockets[leftSockIndex];

	auto rightNode = nodes[rightNodeId];
	auto rightSock = rightNode->inSockets[rightSockIndex];

	// todo: check if socket with pair already exists

	auto con = new ConnectionModel();
	con->leftSocket = leftSock;
	con->rightSocket = rightSock;

	leftSock->connection = con;
	rightSock->connection = con;
	connections.insert(con->id, con);

	return con;
}

void NodeGraph::removeConnection(QString connectionId)
{
	//if (!connections.contains(connectionId))
	//	return;

	auto con = connections[connectionId];

	// assuming it's a complete connection
	con->leftSocket->connection = nullptr;
	con->rightSocket->connection = nullptr;
	connections.remove(connectionId);
}

QJsonObject NodeGraph::serialize()
{
	QJsonObject graph;

	QJsonArray nodesJson;

	// save nodes
	for (auto node : this->nodes.values()) {
		QJsonObject nodeObj;
		nodeObj["id"] = node->id;
		nodeObj["value"] = node->serializeWidgetValue();
		nodeObj["type"] = node->typeName;
		nodesJson.append(nodeObj);
	}
	graph.insert("nodes", nodesJson);

	// save connections
	QJsonArray consJson;
	for (auto con : this->connections.values()) {
		QJsonObject conObj;
		conObj["id"] = con->id;
		conObj["leftNodeId"] = con->leftSocket->node->id;
		conObj["leftNodeSocketIndex"] = con->leftSocket->node->outSockets.indexOf(con->leftSocket);//todo: ugly, cleanup.
		conObj["rightNodeId"] = con->rightSocket->node->id;
		conObj["rightNodeSocketIndex"] = con->rightSocket->node->inSockets.indexOf(con->rightSocket);//todo: ugly, cleanup.

		consJson.append(conObj);
	}
	graph.insert("connections", consJson);
	graph.insert("masternode", this->masterNode->id);

	//todo: save settings (acceptLighting, blendstate, depthstate, etc..)
	graph["settings"] = QJsonObject();

	//todo: save parameters
	QJsonArray propJson;
	for (auto prop : this->properties) {
		QJsonObject propObj = prop->serialize();
		propJson.append(propObj);
	}
	graph["properties"] = propJson;

	return graph;
}

NodeGraph* NodeGraph::deserialize(QJsonObject obj)
{
	auto graph = new NodeGraph();
	registerModels(graph);

	// read settings

	// read properties
	auto graphObj = obj["graph"].toObject();
	auto propList = graphObj["properties"].toArray();
	for (auto propObj : propList) {
		auto prop = Property::parse(propObj.toObject());
		graph->addProperty(prop);
	}

	// read nodes
	auto nodeList = graphObj["nodes"].toArray();
	for (auto nodeVar : nodeList) {
		auto nodeObj = nodeVar.toObject();
		auto type = nodeObj["type"].toString();

		NodeModel* nodeModel = nullptr;
		if (type == "Material") {
			nodeModel = new SurfaceMasterNode();
		}
		else {
			//nodeModel = graph->modelFactories[type]();
			nodeModel = graph->library->createNode(type);
		}
		nodeModel->id = nodeObj["id"].toString();


		// special case for properties
		if (type == "property") {
			auto propId = nodeObj["value"].toString();
			auto prop = graph->getPropertyById(propId);
			((PropertyNode*)nodeModel)->setProperty(prop);
		}
		else {
			nodeModel->deserializeWidgetValue(nodeObj["value"]);
		}

		graph->addNode(nodeModel);
		if (type == "Material") {
			graph->setMasterNode(nodeModel);
		}
	}

	// read connections
	auto conList = graphObj["connections"].toArray();
	for (auto conVar : conList) {
		auto conObj = conVar.toObject();
		auto id = conObj["id"].toString();
		auto leftNodeId = conObj["leftNodeId"].toString();
		auto leftSockIndex = conObj["leftNodeSocketIndex"].toInt();
		auto rightNodeId = conObj["rightNodeId"].toString();
		auto rightSockIndex = conObj["rightNodeSocketIndex"].toInt();

		graph->addConnection(leftNodeId, leftSockIndex, rightNodeId, rightSockIndex);
	}


	return graph;
}
