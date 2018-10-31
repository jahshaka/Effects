#include "socket.h"
#include "socketconnection.h"
#include "graphnodescene.h"
#include <QGraphicsItem>
#include <QFontMetrics>
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>

Socket::Socket(QGraphicsItem* parent, SocketType socketType, QString title) :
	QGraphicsPathItem(parent), 
	socketType(socketType)
{

	this->setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
	text = new QGraphicsTextItem(this);
	text->setPlainText(title);
	text->setDefaultTextColor(QColor(200, 200, 200));

	QFont font = text->font();
	font.setWeight(60);
	auto ratio = QApplication::desktop()->devicePixelRatio();
	font.setPointSize(font.pointSize() * ratio);
	text->setFont(font);

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
		text->setPos(-textRect.width() - radius * 6, -radius);
		if (rounded)  path.addRoundedRect(-radius*4 , -radius / 2, dimentions, dimentions, radius, radius);
		else path.addRect(radius , -radius / 2, dimentions, dimentions);
		socketPos = path.currentPosition();
	}
	else
	{
		//socket on the left      in socket
		text->setPos(radius * 5, -radius);
		if (rounded) path.addRoundedRect(radius*2, -radius / 2, dimentions, dimentions, radius, radius);
		else path.addRect(0, -radius / 2, dimentions, dimentions);

		socketPos = path.currentPosition();
	}
	setBrush(getSocketColor());
	QPen pen(QColor(97, 97, 97, 150), 3.0);
	setPen(pen);
	setPath(path);

	connectedColor = owner->titleColor;
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
	return socketPos.x();
}

int Socket::type() const
{
	return (int)GraphicsItemType::Socket;
}

QPoint Socket::getSocketPosition()
{
	if (socketType == SocketType::Out)
	{
		QRect rect(-radius * 4, -radius / 2, dimentions, dimentions);
		auto center = rect.center();
		return this->scenePos().toPoint() + center;
	}
	else {
		QRect rect(radius * 2, -radius / 2, dimentions, dimentions);
		auto center = rect.center();
		return this->scenePos().toPoint() + center;

	}
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
		if (rounded)  path.addRoundedRect(-radius * 4, -radius / 2, dimentions, dimentions, radius, radius);
		else path.addRect(-radius * 2, -radius / 2, dimentions, dimentions);
	}
	else
	{
		if (rounded) path.addRoundedRect(radius * 2, -radius / 2, dimentions, dimentions, radius, radius);
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

	painter->setRenderHint(QPainter::HighQualityAntialiasing);
	QPainterPath path;
	QPainterPath pathShadow;

	qDebug() << "watching";

	// socket positions are at the outer right or outer left of the graph node
	if (socketType == SocketType::Out)
	{
		path.addRoundedRect(-radius * 4, -radius / 2 , dimentions, dimentions, radius, radius);
		pathShadow.addRoundedRect(-radius * 4, -radius / 2 + 2, dimentions, dimentions, radius, radius);

	}
	else
	{
		path.addRoundedRect(radius * 2, -radius / 2, dimentions, dimentions, radius, radius);
		pathShadow.addRoundedRect(radius * 2, -radius / 2 + 2 , dimentions, dimentions, radius, radius);

	}


	//QGraphicsPathItem::paint(painter, option, widget);



	//fill shadow
	painter->fillPath(pathShadow, QColor(20, 20, 20, 30));

	// fill well
	painter->fillPath(path, QColor(20,20,24,255));


	QPen pen;
	pen.setWidthF(3);
	pen.setColor(QColor(100, 100, 100));
	painter->setPen(pen);
	painter->drawPath(path);

	if (connected) {
		QPainterPath path1;
		QPainterPath path;
		auto pad = 6;

		if (socketType == SocketType::Out)
		{
			path.addRoundedRect(-radius * 4, -radius / 2, dimentions, dimentions, radius, radius);

			path1.addRoundedRect(-radius * 4 + pad/2, -radius / 2 + pad/2, dimentions- pad, dimentions- pad, radius, radius);
		}
		else
		{
			path.addRoundedRect(radius * 2, -radius / 2, dimentions, dimentions, radius, radius);

			path1.addRoundedRect(radius * 2 + pad/2, -radius / 2 + pad/2, dimentions - pad, dimentions - pad, radius, radius);
		}


		QLinearGradient grad;
		grad.setColorAt(1.0, QColor(47, 47, 53));
		grad.setColorAt(0.0, QColor(45, 45, 51));
		grad.setCoordinateMode(QGradient::ObjectBoundingMode);
		painter->fillPath(path, grad);
		painter->fillPath(path1, connectedColor);
	//	painter->drawPath(path);

	}

}