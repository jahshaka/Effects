#include "nodemodel.h"


void NodeGraph::registerModel(NodeModel *model)
{
    modelRegistry.insert(model->title, model);
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

void NodeGraph::addConnection(NodeModel *leftNode, int leftSockIndex, NodeModel *rightNode, int rightSockIndex)
{
    // todo: check if the indices are correct
    addConnection(leftNode->id, leftSockIndex, rightNode->id, rightSockIndex);
}

void NodeGraph::addConnection(QString leftNodeId, int leftSockIndex, QString rightNodeId, int rightSockIndex)
{
    // todo: check if the ids and socket indices are correct
    auto leftNode = nodes[leftNodeId];
    auto leftSock = leftNode->outSockets[leftSockIndex];

    auto rightNode = nodes[rightNodeId];
    auto rightSock = rightNode->inSockets[rightSockIndex];

    // todo: check if socket with pair already exists

    auto con = new ConnectionModel();
    con->leftNodeId = leftNodeId;
    con->leftSocketIndex = leftSockIndex;
    con->rightNodeId = rightNodeId;
    con->rightSocketIndex = rightSockIndex;
    connections.insert(con->id, con);
}

ConnectionModel *NodeGraph::getConnectionFromOutputNode(NodeModel *node, int socketIndex)
{
    for(auto con : connections.values()) {
        if (con->rightNodeId == node->id && con->rightSocketIndex == socketIndex)
            return con;
    }

    return nullptr;
}

ConnectionModel::ConnectionModel()
{
    id = QUuid::createUuid().toString();
    leftSocketIndex = -1;
    rightSocketIndex = -1;
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
}

NodeModel *NodeModel::duplicate()
{
    auto model = new NodeModel;

    model->typeName = this->typeName;
    model->title = this->title;

    for(auto sock : this->inSockets)
        model->inSockets.append(sock->duplicate());
    for(auto sock : this->outSockets)
        model->outSockets.append(sock->duplicate());

    return model;
}
