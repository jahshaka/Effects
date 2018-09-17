#include "nodemodel.h"
#include "graphtest.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

void NodeGraph::addProperty(Property* prop)
{
    this->properties.append(prop);
}

Property *NodeGraph::getPropertyByName(const QString &name)
{
    for(auto prop : properties)
        if (prop->name == name)
            return prop;
    return nullptr;
}

Property *NodeGraph::getPropertyById(const QString &id)
{
    for(auto prop : properties)
        if (prop->id == id)
            return prop;
    return nullptr;
}

void NodeGraph::registerModel(QString name, std::function<NodeModel *()> factoryFunction)
{
    modelFactories.insert(name, factoryFunction);
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

QJsonObject NodeGraph::serialize()
{
    QJsonObject graph;

    QJsonArray nodesJson;

    // save nodes
    for(auto node : this->nodes.values()) {
        QJsonObject nodeObj;
        nodeObj["id"] = node->id;
        nodeObj["value"] = node->serializeWidgetValue();
        nodeObj["type"] = node->typeName;
        nodesJson.append(nodeObj);
    }
    graph.insert("nodes", nodesJson);

    // save connections
    QJsonArray consJson;
    for(auto con : this->connections.values()) {
        QJsonObject conObj;
        conObj["id"] = con->id;
        conObj["leftNodeId"] = con->leftSocket->node->id;
        conObj["leftNodeSocketIndex"] = con->leftSocket->node->outSockets.indexOf(con->leftSocket);//todo: ugly, cleanup.
        conObj["rightNodeId"] = con->rightSocket->node->id;
        conObj["rightNodeSocketIndex"] = con->rightSocket->node->inSockets.indexOf(con->rightSocket);//todo: ugly, cleanup.

        consJson.append(conObj);
    }
    graph.insert("connections", consJson);
    graph.insert("masternode",this->masterNode->id);

    //todo: save settings (acceptLighting, blendstate, depthstate, etc..)
    graph["settings"] = QJsonObject();

    //todo: save parameters
    QJsonArray propJson;
    for(auto prop : this->properties) {
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
    for(auto propObj : propList) {
        auto prop = Property::parse(propObj.toObject());
        graph->addProperty(prop);
    }

    // read nodes
    auto nodeList = graphObj["nodes"].toArray();
    for (auto nodeVar : nodeList) {
        auto nodeObj = nodeVar.toObject();
        auto type = nodeObj["type"].toString();

        NodeModel* nodeModel = nullptr;
        if (type=="Material") {
            nodeModel = new SurfaceMasterNode();
        }
        else {
            nodeModel = graph->modelFactories[type]();
        }
        nodeModel->id = nodeObj["id"].toString();


        // special case for properties
        if (type=="property") {
            auto propId = nodeObj["value"].toString();
            auto prop = graph->getPropertyById(propId);
            ((PropertyNode*)nodeModel)->setProperty(prop);
		}
		else {
			nodeModel->deserializeWidgetValue(nodeObj["value"]);
		}

        graph->addNode(nodeModel);
        if (type=="Material") {
            graph->setMasterNode(nodeModel);
        }
    }

    // read connections
    auto conList = graphObj["connections"].toArray();
    for (auto conVar : conList) {
        auto conObj = conVar.toObject();
        auto id             = conObj["id"].toString();
        auto leftNodeId     = conObj["leftNodeId"].toString();
        auto leftSockIndex  = conObj["leftNodeSocketIndex"].toInt();
        auto rightNodeId    = conObj["rightNodeId"].toString();
        auto rightSockIndex = conObj["rightNodeSocketIndex"].toInt();

        graph->addConnection(leftNodeId, leftSockIndex, rightNodeId, rightSockIndex);
    }


    return graph;
}

/*
ConnectionModel *NodeGraph::getConnectionFromOutputNode(NodeModel *node, int socketIndex)
{
    for(auto con : connections.values()) {
        if (con->rightNodeId == node->id && con->rightSocketIndex == socketIndex)
            return con;
    }

    return nullptr;
}
*/

ConnectionModel::ConnectionModel()
{
    id = QUuid::createUuid().toString();
    leftSocket = nullptr;
    rightSocket = nullptr;
}

void SocketModel::setGraph(NodeGraph *value)
{
    graph = value;
}

NodeGraph *SocketModel::getGraph() const
{
    return graph;
}

QString SocketModel::getVarName() const
{
    return varName;
}

void SocketModel::setVarName(const QString &value)
{
    varName = value;
}

SocketModel *SocketModel::getConnectedSocket()
{
    Q_ASSERT(connection!=nullptr);
    Q_ASSERT(connection->leftSocket != nullptr && connection->rightSocket!=nullptr);

    if(connection->leftSocket==this)
        return connection->rightSocket;

    if(connection->rightSocket==this)
        return connection->leftSocket;

    return nullptr;
}

QString SocketModel::getValue() const
{
    return value;
}

void SocketModel::setValue(const QString &value)
{
    this->value = value;
}

NodeModel *SocketModel::getNode() const
{
    return node;
}

void SocketModel::setNode(NodeModel *value)
{
    node = value;
}

SocketModel::SocketModel()
{
    id = QUuid::createUuid().toString();
}

SocketModel::SocketModel(QString name, QString typeName):
    name(name),
    typeName(typeName)
{
    id = QUuid::createUuid().toString();
}

NodeModel::NodeModel()
{
    id = QUuid::createUuid().toString();
    widget = nullptr;
}

void NodeModel::addInputSocket(SocketModel *sock)
{
    inSockets.append(sock);
    sock->setNode(this);
}

void NodeModel::addOutputSocket(SocketModel *sock)
{
    outSockets.append(sock);
    sock->setNode(this);
}

QString NodeModel::getValueFromInputSocket(int index)
{
    auto sock = inSockets[index];
    if (sock->hasConnection()) {
        //return sock->getConnectedSocket()->getVarName();

        // converts the var before sending it back
        return sock->getConnectedSocket()->convertVarTo(sock);
    }

    return sock->getValue();
}

QString NodeModel::getOutputSocketVarName(int index)
{
    auto sock = outSockets[index];
    return sock->getVarName();
}

NodeGraph *NodeModel::getGraph() const
{
    return graph;
}

void NodeModel::setGraph(NodeGraph *value)
{
    graph = value;
}

/*
NodeModel *NodeModel::duplicate()
{
    auto model = createDuplicate();
    
    model->typeName = this->typeName;
    model->title = this->title;

    for(auto sock : this->inSockets)
        model->inSockets.append(sock->duplicate());
    for(auto sock : this->outSockets)
        model->outSockets.append(sock->duplicate());

    return model;
}
*/
