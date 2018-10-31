#include "socketconnection.h"
#include "socket.h"
#include <QPainter>
#include <QDebug>
#include <QGraphicsDropShadowEffect>


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

	QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
	effect->setBlurRadius(20);
	effect->setXOffset(0);
	effect->setYOffset(0);
	effect->setColor(QColor(00, 100, 200, 140));
	setGraphicsEffect(effect);
}

void SocketConnection::updatePosFromSockets()
{
	pos1 = socket1->getSocketPosition();
	pos2 = socket2->getSocketPosition();
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

	
	setPath(*p);

	
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
	if(status == SocketConnectionStatus::Finished) {
		//QGraphicsPathItem::paint(painter, option, widget);
		QLinearGradient grad;
	//	grad.setFocalPoint(QPoint(0,0));
	//	grad.setCenter(QPoint(1, 1));
	//	grad.setRadius(1);
		grad.setStart(pos1-pos2);
		grad.setFinalStop( pos2 - pos1);
	//	grad.setCoordinateMode(QGradient::StretchToDeviceMode);
		grad.setColorAt(0.0, socket1->connectedColor);
		grad.setColorAt(1.0, socket2->connectedColor);

		QPen pen(grad,2);

		painter->setPen(pen);
		painter->drawPath(*p);

	}


}

