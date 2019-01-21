#pragma once
#include <QUndoStack>
#include "../graphnode.h"

class UndoRedo : public QUndoCommand
{
public:
	UndoRedo();
	~UndoRedo();

	void undo() override;
	void redo() override;
};

class AddNodeCommand : public UndoRedo
{
	AddNodeCommand(GraphNode *node);

};

class AddConnectionCommand : public UndoRedo
{
	AddConnectionCommand(GraphNode *node);

};

class MoveNodeCommand : public UndoRedo
{
	MoveNodeCommand(GraphNode *node);

};

class DeleteNodeCommand : public UndoRedo
{
	DeleteNodeCommand(GraphNode *node);

};

