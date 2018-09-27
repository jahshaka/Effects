#include "graphicsview.h"
#include <QDebug>
#include <QDragEnterEvent>
#include <QMimeData>
GraphicsView::GraphicsView( QWidget *parent) : QGraphicsView(parent)
{
	setAcceptDrops(true);
	QGraphicsView::setAcceptDrops(true);
}

void GraphicsView::dragEnterEvent(QDragEnterEvent * event)
{
	event->setAccepted(true);
}

void GraphicsView::dropEvent(QDropEvent * event)
{
	event->acceptProposedAction();
	QGraphicsView::dropEvent(event);
}

void GraphicsView::dragMoveEvent(QDragMoveEvent * event)
{
	event->setAccepted(true);
}
