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

	Socket* socket1;
	Socket* socket2;

	QPointF pos1;
	QPointF pos2;

	SocketConnectionStatus status;
	QPainterPath* p;
	float lineThickness = 3.0;

	SocketConnection();

	void updatePosFromSockets();
	void updatePath();
	virtual int type() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR) override;
	void hoverEnterEvent(QGraphicsSceneHoverEvent * event) override;
	bool sceneEventFilter(QGraphicsItem *watched, QEvent *event) override;
};
