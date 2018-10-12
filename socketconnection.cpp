#include "socketconnection.h"
#include "socket.h"
#include <QPainter>



SocketConnection::SocketConnection()
{
	socket1 = nullptr;
	socket2 = nullptr;

	pos1 = QPointF(0, 0);
	pos2 = QPointF(0, 0);

	setFlag(QGraphicsItem::ItemAcceptsInputMethod);
	setFlag(QGraphicsItem::ItemIsSelectable);
	setFlag(QGraphicsItem::ItemIsMovable);

	auto pen = QPen(QColor(200, 200, 200));
	pen.setBrush(QColor(50, 150, 250));
	pen.setCapStyle(Qt::RoundCap);
	pen.setWidth(3);
	setPen(pen);
}

void SocketConnection::updatePosFromSockets()
{
	pos1 = socket1->scenePos() + QPoint(socket1->getSocketOffset(), 5);
	pos2 = socket2->scenePos() - QPoint(socket1->getSocketOffset() * 2, -5);
}

void SocketConnection::updatePath()
{

	p = new QPainterPath;
	p->moveTo(pos1);

	qreal dx = pos2.x() - pos1.x();
	qreal dy = pos2.y() - pos1.y();

	QPointF ctr1(pos1.x() + dx * 0.25, pos1.y() + dy * 0.1);
	QPointF ctr2(pos1.x() + dx * 0.75, pos1.y() + dy * 0.9);

	p->cubicTo(ctr1, ctr2, pos2);
	p->setFillRule(Qt::OddEvenFill);

	//if (status == SocketConnectionStatus::Started || status == SocketConnectionStatus::Inprogress) {
	//	QPainterPathStroker str;
	//	str.setCapStyle(Qt::RoundCap);
	//	str.setWidth(10.0);
	//	str.setDashPattern(Qt::CustomDashLine);
	//	str.setDashOffset(19);
	//	QPainterPath resultPath = str.createStroke(p).simplified();


	//	setPath(resultPath);
	//}
	//else {

	setPath(*p);

	//}
}

int SocketConnection::type() const
{
	return (int)GraphicsItemType::Connection;
}

void SocketConnection::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	painter->setRenderHint(QPainter::Antialiasing);
	if (status == SocketConnectionStatus::Started || status == SocketConnectionStatus::Inprogress) {
		QPen pen(QColor(90, 90, 90), 2);
		pen.setStyle(Qt::DashLine);
		pen.setDashOffset(6);
		painter->setPen(pen);
		painter->drawPath(*p);
	}
	else {
		QGraphicsPathItem::paint(painter, option, widget);
	}


}

