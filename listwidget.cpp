#include "listwidget.h"
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>
#include <QPixmap>
#include <QScrollBar>
#include <QDebug>
#include <QLayout>

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
	setDefaultDropAction(Qt::CopyAction);
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
	
	QFont font = this->font();
	font.setWeight(60);
	font.setPixelSize(12);
	setFont(font);

	verticalScrollBar()->setStyleSheet(
		"QScrollBar:vertical {border : 0px solid black;	background: rgba(132, 132, 132, 0);width: 10px; }"
		"QScrollBar::handle{ background: rgba(72, 72, 72, 1);	border-radius: 5px;  left: 8px; }"
		"QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {	background: rgba(200, 200, 200, 0);}"
		"QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical {	background: rgba(0, 0, 0, 0);border: 0px solid white;}"
		"QScrollBar::sub-line, QScrollBar::add-line {	background: rgba(10, 0, 0, .0);}"
		
	);

	setStyleSheet(
		"QListView::item{ border-radius: 2px; border: 1px solid rgba(0,0,0,1); background: rgba(80,80,80,1); margin: 3px;  }"
		"QListView::item:selected{ background: rgba(65,65,65,1); border: 1px solid rgba(50,150,250,1); }"
		"QListView::item:hover{ background: rgba(55,55,55,1); border: 1px solid rgba(50,150,250,1); }"
		"QListView::text{ top : -6; }"
	);
	
}


ListWidget::~ListWidget()
{
}

void ListWidget::displayAllContents()
{

	int num_of_items_per_row = width() / (gridSize().width()+6);
	int num_of_models = model()->rowCount();
	int number_of_rows = num_of_models / num_of_items_per_row;
	if (num_of_models % num_of_items_per_row != 0) number_of_rows++;
	int calculated_height = number_of_rows * (gridSize().height() + num_of_items_per_row*2);

	setFixedHeight(calculated_height);

}




QMimeData * ListWidget::mimeData(const QList<QListWidgetItem *> items) const
{
	QMimeData *data = new QMimeData();

	data->setText(items[0]->data(Qt::UserRole).toString());
	return data;
}

void ListWidget::resizeEvent(QResizeEvent * event)
{
	QListWidget::resizeEvent(event);
	if(isResizable) displayAllContents();
}
