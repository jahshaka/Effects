#include "undoredo.h"
#include <QPoint>


UndoRedo::UndoRedo()
{
}


UndoRedo::~UndoRedo()
{
}

void UndoRedo::undo()
{
}

void UndoRedo::redo()
{
}

AddNodeCommand::AddNodeCommand(GraphNode * node, GraphNodeScene* scene)
{
	initialPosition = node->pos();
	this->scene = scene;
	this->node = node;
	setText(QObject::tr("Add %1").arg(node->nodeType + "command"));
}

void AddNodeCommand::undo()
{
	scene->deleteNode(node);
}

void AddNodeCommand::redo()
{
	scene->addItem(node);
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
}

void AddConnectionCommand::redo()
{
	scene->addConnection(connection->socket1->owner->nodeId, left, connection->socket2->owner->nodeId, right);
	//remind nick to add connections to models!

}


MoveNodeCommand::MoveNodeCommand(GraphNode * node, GraphNodeScene *scene, QPointF oldPos, QPointF newPos)
{
	this->scene = scene;
	this->node = node;
	this->oldPos = oldPos;
	this->newPos = newPos;
}

void MoveNodeCommand::undo() {
	node->setPos(oldPos);
}


void MoveNodeCommand::redo() {
	node->setPos(newPos);
}


DeleteNodeCommand::DeleteNodeCommand(GraphNode * node, GraphNodeScene *scene)
{
	this->node = node;
	this->scene = scene;
	initialPosition = node->pos();
	connections = scene->nodeGraph->getNodeConnections(node->nodeId);
}

void DeleteNodeCommand::undo()
{

}

void DeleteNodeCommand::redo()
{
	scene->deleteNode(node);
}

MoveMultipleCommand::MoveMultipleCommand(QList<GraphNode*>& list, GraphNodeScene *scene)
{
	this->scene = scene;
	this->nodes = list;
}

void MoveMultipleCommand::undo()
{
	for (auto node : nodes) node->setPos(node->initialPoint);
}

void MoveMultipleCommand::redo()
{
	for (auto node : nodes) node->setPos(node->movedPoint);
}
