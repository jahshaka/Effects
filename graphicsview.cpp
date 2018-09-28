#include "graphicsview.h"
#include <QDebug>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QPainter>

qreal GraphicsView::currentScale = 1.0;
GraphicsView::GraphicsView( QWidget *parent) : QGraphicsView(parent)
{
	setAcceptDrops(true);
	setRenderHint(QPainter::Antialiasing);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setCacheMode(QGraphicsView::CacheBackground);
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	QGraphicsView::setAcceptDrops(true);
}

void GraphicsView::increaseScale()
{
	double const step = 1.2;
	double const factor = std::pow(step, 1.0);

	QTransform t = transform();

	if (t.m11() > 2.0)
		return;

	scale(factor, factor);
}

void GraphicsView::decreaseScale()
{
	double const step = 1.2;
	double const factor = std::pow(step, -1.0);

	scale(factor, factor);
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

void GraphicsView::drawBackground(QPainter * painter, const QRectF & rect)
{
	painter->setRenderHint(QPainter::Antialiasing);
	QGraphicsView::drawBackground(painter, rect);

	auto drawGrid =
		[&](double gridStep)
	{
		QRect   windowRect = this->rect();
		QPointF tl = mapToScene(windowRect.topLeft());
		QPointF br = mapToScene(windowRect.bottomRight());

		double left = std::floor(tl.x() / gridStep - 0.5);
		double right = std::floor(br.x() / gridStep + 1.0);
		double bottom = std::floor(tl.y() / gridStep - 0.5);
		double top = std::floor(br.y() / gridStep + 1.0);

		QPen pen(QColor(200, 200, 200), 2);
		painter->setPen(pen);

		// vertical lines
		for (int xi = int(left); xi <= int(right); ++xi)
		{
			QLineF line(xi * gridStep, bottom * gridStep,
				xi * gridStep, top * gridStep);

			painter->drawLine(line);
		}

		// horizontal lines
		for (int yi = int(bottom); yi <= int(top); ++yi)
		{
			QLineF line(left * gridStep, yi * gridStep,
				right * gridStep, yi * gridStep);
			painter->drawLine(line);
		}
	};

	drawGrid(35);

}

void GraphicsView::wheelEvent(QWheelEvent * event)
{
	QPoint delta = event->angleDelta();

	if (delta.y() == 0)
	{
		event->ignore();
		return;
	}

	double const d = delta.y() / std::abs(delta.y());

	if (d > 0.0)
		increaseScale();
	else
		decreaseScale();

	QGraphicsView::wheelEvent(event);
}
