#include "undoredo.h"



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

AddConnectionCommand::AddConnectionCommand(GraphNode * node)
{
}

MoveNodeCommand::MoveNodeCommand(GraphNode * node)
{
}

DeleteNodeCommand::DeleteNodeCommand(GraphNode * node)
{
}
