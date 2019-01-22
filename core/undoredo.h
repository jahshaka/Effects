#pragma once
#include <QUndoStack>
#include <QPoint>
#include "../graphnode.h"
#include "../graphnodescene.h"

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
public:
	AddNodeCommand(GraphNode *node, GraphNodeScene *);

	void undo() override;
	void redo() override;

private :
	QPointF initialPosition;
	GraphNodeScene* scene;
	GraphNode* node;
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

