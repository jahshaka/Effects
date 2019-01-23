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
public:
	AddConnectionCommand(SocketConnection *, GraphNodeScene *, int leftSocket, int rightSocket);

	void undo() override;
	void redo() override;
private:
	int left, right;
	QString connectionID;
	GraphNodeScene* scene;
	GraphNode* node;
	SocketConnection* connection;
};

class MoveNodeCommand : public UndoRedo
{
public:
	MoveNodeCommand(GraphNode *node, GraphNodeScene *,  QPointF oldPos, QPointF newPos);

	void undo() override;
	void redo() override;
private:
	QPointF oldPos;
	QPointF newPos;
	GraphNode* node;
	GraphNodeScene* scene;
};

class MoveMultipleCommand : public UndoRedo
{
public:
	MoveMultipleCommand(QList<GraphNode*> &, GraphNodeScene*);

	void undo() override;
	void redo() override;
private:
	QList<GraphNode*> nodes;
	GraphNodeScene* scene;

};

class DeleteNodeCommand : public UndoRedo
{
public:
	DeleteNodeCommand(GraphNode *node, GraphNodeScene *);

	void undo() override;
	void redo() override;
private:
	QPointF initialPosition;
	GraphNodeScene* scene;
	GraphNode* node;
	QVector<ConnectionModel*> connections;


};

