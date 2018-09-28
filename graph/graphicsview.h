#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QWidget>
#include <QGraphicsView>

class GraphicsView : public QGraphicsView
{
public:
    GraphicsView(QWidget *parent = Q_NULLPTR);
	static qreal currentScale;

private:
	void increaseScale();
	void decreaseScale();

protected:
	void dragEnterEvent(QDragEnterEvent *event) override;
	void dropEvent(QDropEvent *event) override;
	void dragMoveEvent(QDragMoveEvent *event) override;
	void drawBackground(QPainter *painter, const QRectF &rect);
	void wheelEvent(QWheelEvent *event) ;

};

#endif // GRAPHICSVIEW_H