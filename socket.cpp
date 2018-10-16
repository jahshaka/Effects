#include "socket.h"
#include "socketconnection.h"
#include "graphnodescene.h"
#include <QGraphicsItem>
#include <QFontMetrics>

Socket::Socket(QGraphicsItem* parent, SocketType socketType, QString title) :
	QGraphicsPathItem(parent), 
	socketType(socketType)
{

	this->setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
	text = new QGraphicsTextItem(this);
	text->setPlainText(title);
	text->setDefaultTextColor(QColor(200, 200, 200));
	setSocketColor(disconnectedColor);
	QFontMetrics fm(parent->scene()->font());
	QRect textRect = fm.boundingRect(title);
	owner = (GraphNode*)parent;
	radius = qCeil(textRect.height() / 2.0f);
	dimentions = textRect.height();
	QPainterPath path;

	// socket positions are at the outer right or outer left of the graph node
	if (socketType == SocketType::Out)
	{
		// socket on the right    out socket
		text->setPos(-textRect.width() - radius * 4, -radius);
		if (rounded)  path.addRoundedRect(-radius * 2, -radius / 2, dimentions, dimentions, radius, radius);
		else path.addRect(-radius * 2, -radius / 2, dimentions, dimentions);
		socketPos = path.currentPosition();
	}
	else
	{
		//socket on the left      in socket
		text->setPos(radius * 3, -radius);
		if (rounded) path.addRoundedRect(0, -radius / 2, dimentions, dimentions, radius, radius);
		else path.addRect(0, -radius / 2, dimentions, dimentions);

		socketPos = path.currentPosition();
	}
	setBrush(getSocketColor());
	QPen pen(QColor(97, 97, 97), 3);
	setPen(pen);
	setPath(path);
}

void Socket::addConnection(SocketConnection* con)
{
	setConnected(true);
	connections.append(con);
	setSocketColor(connectedColor);
	text->setDefaultTextColor(QColor(255, 255, 255));
	updateSocket();
}

void Socket::removeConnection(SocketConnection * con)
{
	connections.removeOne(con);
	if (connections.length() <= 0) {
		setSocketColor(disconnectedColor);
		setConnected(false);
	}
	text->setDefaultTextColor(QColor(200, 200, 200));
	updateSocket();
}

float Socket::calcHeight()
{
	return radius * 2;
}

float Socket::getRadius()
{
	return radius;
}

QPointF Socket::getPos()
{
	return socketPos;
}

float Socket::getSocketOffset()
{
	return socketPos.x() / 2;
}

int Socket::type() const
{
	return (int)GraphicsItemType::Socket;
}

QVariant Socket::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if (change == ItemScenePositionHasChanged)
	{
		for (auto con : connections)
		{
			con->updatePosFromSockets();
			con->updatePath();
		}
	}
	return value;
}

QColor Socket::getSocketColor()
{
	return socketColor;
}

void Socket::setSocketColor(QColor color)
{
	socketColor = color;
}

void Socket::setConnected(bool value)
{
	connected = value;
}

QPainterPath Socket::addInvisibleCover()
{
	QPainterPath path;
	if (socketType == SocketType::Out)
	{
		if (rounded)  path.addRoundedRect(-radius-8 * 2, -radius-4 / 2, dimentions*1.6, dimentions*1.6, radius*2, radius*2);
		else path.addRect(-radius * 2, -radius / 2, dimentions, dimentions);
	}
	else
	{
		if (rounded) path.addRoundedRect(-6, -radius-4 / 2, dimentions*1.6, dimentions*1.6, radius*2, radius*2);
		else path.addRect(0, -radius / 2, dimentions, dimentions);
	}
	setBrush(QColor(200, 100, 100));
	return path;
}

void Socket::updateSocket()
{
	QPainterPath path;
	// socket positions are at the outer right or outer left of the graph node
	if (socketType == SocketType::Out)
	{
		if (rounded)  path.addRoundedRect(-radius * 2, -radius / 2, dimentions, dimentions, radius, radius);
		else path.addRect(-radius * 2, -radius / 2, dimentions, dimentions);
	}
	else
	{
		if (rounded) path.addRoundedRect(0, -radius / 2, dimentions, dimentions, radius, radius);
		else path.addRect(0, -radius / 2, dimentions, dimentions);
	}
	setBrush(getSocketColor());
	QPen pen;
	pen.setWidth(3);
	if (connected) pen.setColor(QColor(27, 27, 27));
	else pen.setColor(QColor(97, 97, 97));

	setPen(pen);
	setPath(path);
}

void Socket::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	QGraphicsPathItem::paint(painter, option, widget);

}