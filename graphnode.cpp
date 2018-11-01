#include "graphnode.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsEffect>
#include "socket.h"
#include "socketconnection.h"
#include <QPixmap>
#include <QGraphicsPixmapItem>

GraphNode::GraphNode(QGraphicsItem* parent) :
	QGraphicsPathItem(parent)
{


	nodeType = 0;
	proxyWidget = nullptr;


	this->setFlag(QGraphicsItem::ItemIsMovable);
	this->setFlag(QGraphicsItem::ItemIsSelectable);
	this->setFlag(QGraphicsItem::ItemSendsGeometryChanges);
	this->setFlag(QGraphicsItem::ItemSendsScenePositionChanges);

	nodeWidth = 170;
	QPainterPath path_content;
	// path_content.setFillRule(Qt::WindingFill);
	//path_content.addRoundedRect(QRect(0, 0, 100, 220), 5, 5);
	//path_content.addRect(QRect(0, 0, nodeWidth, calcHeight()));
	//path_content.addRoundedRect(QRect(0, 0, nodeWidth, calcHeight()),17,17);
	setPath(path_content);

	setPen(QPen(Qt::black));
	setBrush(QColor(240, 240, 240));

	text = new QGraphicsTextItem(this);
	text->setPlainText("Title");
	text->setPos(5, 2);
	text->setDefaultTextColor(QColor(255, 255, 255));

	QFont font = text->font();
	font.setWeight(65);
	auto ratio = QApplication::desktop()->devicePixelRatio();
	font.setPointSize(font.pointSize() * ratio);
	text->setFont(font);

	QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
	effect->setBlurRadius(12);
	effect->setXOffset(0);
	effect->setYOffset(0);
	effect->setColor(QColor(00, 00, 00, 40));
	setGraphicsEffect(effect);

}

void GraphNode::setIcon(QIcon icon)
{
	this->icon = icon;
}

void GraphNode::setTitleColor(QColor color)
{
	titleColor = color;

}

void GraphNode::setTitle(QString title)
{
	text->setPlainText(title);
	auto textWidth = text->boundingRect().width();
	text->setPos(nodeWidth / 2 - textWidth / 2, 2);
}

void GraphNode::addInSocket(QString title)
{
	auto sock = new Socket(this, SocketType::In, title);
	auto y = calcHeight();// + sock->calcHeight();
	sock->setPos(-sock->getRadius(), y);
	sock->node = this;
	sock->socketIndex = inSocketCount++;
	addSocket(sock);
}

void GraphNode::addOutSocket(QString title)
{
	auto sock = new Socket(this, SocketType::Out, title);
	auto y = calcHeight();// + sock->calcHeight();
	sock->setPos(nodeWidth + sock->getRadius(), y);
	sock->node = this;
	sock->socketIndex = outSocketCount++;
	addSocket(sock);
}

void GraphNode::addSocket(Socket* sock)
{
	sockets.append(sock);

	// recalc path
	calcPath();
}

void GraphNode::setWidget(QWidget *widget)
{
	// gotta do this here before adding the widget
	auto y = calcHeight();

	proxyWidget = new QGraphicsProxyWidget(this);
	proxyWidget->setWidget(widget);
	//proxyWidget->setPreferredSize(widget->pr);
	proxyWidget->setPreferredWidth(5);
	proxyWidget->setPos((nodeWidth - proxyWidget->size().width()) / 2,
		y);

	calcPath();
}

void GraphNode::calcPath()
{
	QPainterPath path_content;
	path_content.setFillRule(Qt::WindingFill);
	path_content.addRoundedRect(QRect(0, 0, nodeWidth, calcHeight()), titleRadius, titleRadius);
	setPath(path_content);


}

int GraphNode::calcHeight()
{
	int height = 0;
	height += titleHeight + 20;// title + padding

	for (auto socket : sockets)
	{
		height += socket->calcHeight();
		height += 14; // padding
	}

	//height += 2; // padding
	if (proxyWidget != nullptr)
		height += proxyWidget->size().height();

	return height;
}

Socket *GraphNode::getInSocket(int index)
{
	int i = 0;
	for (auto sock : sockets) {
		if (sock->socketType == SocketType::In) {
			if (index == i)
				return sock;
			i++;
		}
	}

	return nullptr;
}

Socket *GraphNode::getOutSocket(int index)
{
	int i = 0;
	for (auto sock : sockets) {
		if (sock->socketType == SocketType::Out) {
			if (index == i)
				return sock;
			i++;
		}
	}
}

void GraphNode::paint(QPainter *painter,
	const QStyleOptionGraphicsItem *option,
	QWidget *widget)
{
	painter->setRenderHint(QPainter::HighQualityAntialiasing);
	painter->setRenderHint(QPainter::Antialiasing);
	painter->setRenderHint(QPainter::TextAntialiasing);
	painter->setRenderHint(QPainter::SmoothPixmapTransform);

	if (option->state.testFlag(QStyle::State_Selected) != currentSelectedState) {
		currentSelectedState = option->state.testFlag(QStyle::State_Selected);
		highlightNode(currentSelectedState, 0);
	}

	if (isHighlighted && level == 0) {
		auto rect = boundingRect();
		painter->setPen(QPen(connectedColor, 3));
		painter->drawRoundedRect(rect, titleRadius, titleRadius);
	}
	else if (isHighlighted && level > 0) {
		auto rect = boundingRect();
		painter->setPen(QPen(QColor(250, 250, 50), 3));
		painter->drawRoundedRect(rect, titleRadius, titleRadius);
	}

	painter->setPen(pen());
	setBrush(QColor(45, 45, 51));
	painter->fillPath(path(), brush());

	// title tab
	QPainterPath titlePath;
	titlePath.setFillRule(Qt::WindingFill);
	//titlePath.addRoundedRect(0, 0, nodeWidth, 35, 7, 7);
	titlePath.addRect(0, 10, nodeWidth, titleHeight-10);
	titlePath.addRoundedRect(0, 0, nodeWidth, titleHeight, titleRadius, titleRadius);
	painter->fillPath(titlePath, QBrush(titleColor));

	//draw icon
	//painter->drawPixmap(5, 5, 15, 15, icon.pixmap(50, 50));
	
	

	//draw text node seperator
	QPainterPath block;
	block.setFillRule(Qt::WindingFill);
	block.addRect(0, titleHeight , nodeWidth, 3);
	painter->fillPath(block, QBrush(QColor(30, 30, 30, 160)));

	QPen pen(QColor(00, 00, 00, 250), .5);
	painter->setPen(pen);
//	painter->drawRoundedRect(boundingRect(), titleRadius, titleRadius);

	

}

int GraphNode::type() const
{
	return (int)GraphicsItemType::Node;
}

void GraphNode::highlightNode(bool val, int lvl)
{
	isHighlighted = val;
	level = lvl;

	auto setLevel = [val, lvl]() {
		int i = 0;
		if (lvl == 0)  i = 0;
		else  i = lvl + 1;
		return i;

	};

	for (Socket* sock : sockets) {
		if (sock->socketType == SocketType::In) {
			for (SocketConnection* con : sock->connections) {
				if (con->socket1->socketType == SocketType::Out) {
					con->socket1->owner->isHighlighted = val;
					con->socket1->owner->highlightNode(val, level + 1);
					con->socket1->owner->currentSelectedState = false;
				}
				if (con->socket2->socketType == SocketType::Out) {
					con->socket2->owner->isHighlighted = val;
					con->socket2->owner->highlightNode(val, level + 1);
					con->socket2->owner->currentSelectedState = false;

				}
			}
		}
	}
	if (!val) check = val;
	update();
}

void GraphNode::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	check = true;
	QGraphicsPathItem::mousePressEvent(event);

}
