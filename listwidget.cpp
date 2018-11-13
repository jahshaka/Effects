#include "listwidget.h"
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>
#include <QPixmap>
#include <QScrollBar>

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
	verticalScrollBar()->setStyleSheet(
		"QScrollBar:vertical {border : 0px solid black;	background: rgba(132, 132, 132, 0);width: 10px; }"
		"QScrollBar::handle{ background: rgba(72, 72, 72, 1);	border-radius: 5px;  left: 8px; }"
		"QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {	background: rgba(200, 200, 200, 0);}"
		"QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical {	background: rgba(0, 0, 0, 0);border: 0px solid white;}"
		"QScrollBar::sub-line, QScrollBar::add-line {	background: rgba(10, 0, 0, .0);}"
	);
	
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