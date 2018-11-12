#include "listwidget.h"
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>
#include <QPixmap>

ListWidget::ListWidget() : QListWidget()
{
	QSize currentSize(100, 100);
	setAlternatingRowColors(false);
	setSpacing(0);
	setContentsMargins(10, 3, 10, 10);
	setViewMode(QListWidget::IconMode);
	setIconSize(currentSize);
	setMouseTracking(true);
	setDragDropMode(QAbstractItemView::DragDrop);
	setMovement(QListView::Static);
	setResizeMode(QListWidget::Adjust);
	setDefaultDropAction(Qt::MoveAction);
	setSelectionMode(QAbstractItemView::SingleSelection);
	setSelectionRectVisible(false);
	setDragEnabled(true);
	viewport()->setAcceptDrops(false);
	setDropIndicatorShown(true);
	installEventFilter(this);
	//viewport()->installEventFilter(this);
	setWordWrap(true);
	setGridSize(QSize(90, 90));
	setSortingEnabled(true);
	sortItems();
	setEditTriggers(QAbstractItemView::NoEditTriggers);
}


ListWidget::~ListWidget()
{
}


QMimeData * ListWidget::mimeData(const QList<QListWidgetItem *> items) const
{
	QMimeData *data = new QMimeData();

	data->setText(items[0]->data(Qt::UserRole).toString());
	return data;
}