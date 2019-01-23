#include "undoredo.h"
#include <QPoint>
#include <QDebug>

int UndoRedo::count = 0;
UndoRedo::UndoRedo()
{

}


UndoRedo::~UndoRedo()
{
}

void UndoRedo::undo()
{
	count--;
	qDebug() << count;

}

void UndoRedo::redo()
{
	count++;
	qDebug() << count;

}

AddNodeCommand::AddNodeCommand(NodeModel * nodeModel, GraphNodeScene* scene)
{
	initialPosition = {nodeModel->getX(), nodeModel->getY()};
	this->scene = scene;
	this->nodeModel = nodeModel;
	setText(QObject::tr("Add %1").arg(nodeModel->title + "command"));
}

void AddNodeCommand::undo()
{
	auto node = scene->getNodeById(nodeModel->id);
	scene->deleteNode(node);
	scene->update();
	UndoRedo::undo();
}

void AddNodeCommand::redo()
{
	scene->addNodeModel(nodeModel, nodeModel->getX(), nodeModel->getY());
	UndoRedo::redo();

}

AddConnectionCommand::AddConnectionCommand(SocketConnection * conn, GraphNodeScene *scene, int leftSocket, int rightSocket)
{
	this->connection = conn;
	this->scene = scene;
	this->left = leftSocket;
	this->right = rightSocket;
	connectionID = conn->connectionId;
	setText(QObject::tr("Add %1").arg("connection command"));

}

void AddConnectionCommand::undo()
{

	scene->removeConnection(connectionID, false, true);
	//remind nick to remove connections from models!

	UndoRedo::undo();

}

void AddConnectionCommand::redo()
{
	scene->addConnection(connection->socket1->owner->nodeId, left, connection->socket2->owner->nodeId, right);
	//remind nick to add connections to models!
	UndoRedo::redo();

}

DeleteNodeCommand::DeleteNodeCommand(QList<GraphNode*> & list, GraphNodeScene *scene)
{
	this->list = list;
	this->scene = scene;
}

void DeleteNodeCommand::undo()
{
	for (auto node : list) {
		auto conn = scene->nodeGraph->getNodeConnections(node->nodeId);
		scene->nodeGraph->addNode(node->model);

		//add connections
		for (auto con : conn) {
			auto leftNode = con->leftSocket->node;
			auto rightNode = con->rightSocket->node;
			scene->addConnection(leftNode->id,
				leftNode->outSockets.indexOf(con->leftSocket),
				rightNode->id,
				rightNode->inSockets.indexOf(con->rightSocket));
		}

		scene->addItem(node);
	}

	UndoRedo::undo();

}

void DeleteNodeCommand::redo()
{
	for (auto node : list) {
		scene->deleteNode(node);
	}
	UndoRedo::redo();

}

MoveMultipleCommand::MoveMultipleCommand(QList<GraphNode*>& list, GraphNodeScene *scene)
{
	this->scene = scene;
	this->nodes = list;
	for (auto node : nodes) {
		map.insert(node->nodeId, { node->initialPoint, node->movedPoint });
	}
}

void MoveMultipleCommand::undo()
{
	for (auto node : nodes) {
		node->setPos(map.value(node->nodeId).first);
		node->model->setX(map.value(node->nodeId).first.x());
		node->model->setY(map.value(node->nodeId).first.y());
	}
	UndoRedo::undo();

}

void MoveMultipleCommand::redo()
{
	QList<GraphNode*> nodeList;
	for (auto node : nodes) {
		auto actualNode = scene->getNodeById(node->nodeId);
		if(actualNode) nodeList.append(actualNode);
	}


	for (auto node : nodeList) {
//		if (node->pos() != node->movedPoint) {
			node->setPos(map.value(node->nodeId).second);
			node->model->setX(map.value(node->nodeId).second.x());
			node->model->setY(map.value(node->nodeId).second.y());
			qDebug() << map.value(node->nodeId).first << map.value(node->nodeId).second;
			qDebug() << node << node->nodeId;
//		}

	}
		
	UndoRedo::redo();

}
