#pragma once

#include <QGraphicsPathItem>



enum class SocketConnectionStatus
{
	Started,
	Inprogress,
	Finished,
	Cancelled,
	Editing,
};

class Socket;
class GraphNode;
class SocketConnection : public QGraphicsPathItem
{
public:
	QString connectionId;

	// these may not always be in the order of left and right
	Socket* socket1;
	Socket* socket2;

	QPointF pos1;
	QPointF pos2;

	SocketConnectionStatus status;
	QPainterPath* p;
    double lineThickness = 5.0;

	SocketConnection();

	Socket* getInSocket();
	Socket* getOutSocket();

	void updatePosFromSockets();
	void updatePath();
	virtual int type() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR) override;

};
