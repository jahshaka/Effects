#include "listwidget.h"
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>
#include <QPixmap>
#include <QScrollBar>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLayout>
#include <QMenu>
#include "core/project.h"


ListWidget::ListWidget() : QListWidget()
{
	QSize currentSize(70, 70);
	setAlternatingRowColors(false);
	setSpacing(0);
	setContentsMargins(10, 3, 10, 10);
	setViewMode(QListWidget::IconMode);
	setIconSize(currentSize);
	setMouseTracking(true);
	setDragDropMode(QAbstractItemView::DragDrop);
	//setMovement(QListView::Static);
	setResizeMode(QListWidget::Adjust);
	setDefaultDropAction(Qt::CopyAction);
	setSelectionMode(QAbstractItemView::SingleSelection);
	setSelectionRectVisible(false);
	setDragEnabled(true);
	viewport()->setAcceptDrops(true);
	setAcceptDrops(true);
	setDropIndicatorShown(true);
	//installEventFilter(this);
	//viewport()->installEventFilter(this);
	setWordWrap(true);
	setGridSize(QSize(90, 90));
	setSortingEnabled(true);
	sortItems();
    setEditTriggers(QAbstractItemView::EditKeyPressed);
    setContextMenuPolicy(Qt::CustomContextMenu);

	numberOfItemPerRow = 3;
	QFont font = this->font();
	font.setWeight(60);
	font.setPixelSize(12);
	setFont(font);
	setContentsMargins(0, 0, 0, 0);
	verticalScrollBar()->setStyleSheet(
		"QScrollBar:vertical {border : 0px solid black;	background: rgba(132, 132, 132, 0);width: 10px; }"
		"QScrollBar::handle{ background: rgba(72, 72, 72, 1);	border-radius: 3px;  left: 8px; }"
		"QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {	background: rgba(200, 200, 200, 0);}"
		"QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical {	background: rgba(0, 0, 0, 0);border: 0px solid white;}"
		"QScrollBar::sub-line, QScrollBar::add-line {	background: rgba(10, 0, 0, .0);}"
		
	);

	setStyleSheet(
		"QListView::item{ border-radius: 2px; border: 0px solid rgba(0,0,0,1); background: rgba(80,80,80,0); margin-left: 6px;  }"
		"QListView::item:selected{ background: rgba(65,65,65,1); border: 1px solid rgba(50,150,250,1); }"
		"QListView::item:hover{ background: rgba(55,55,55,1); border: 1px solid rgba(50,150,250,1); }"
		"QListView::text{ top : -6; }"
	);

    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),this, SLOT(customContextMenu(const QPoint&)));
	
}


ListWidget::~ListWidget()
{
}

void ListWidget::displayAllContents()
{
	setGridSize(QSize(95, 95));

	int num_of_items_per_row = width() / (gridSize().width()+6);
	int num_of_models = model()->rowCount();
	int number_of_rows = num_of_models / num_of_items_per_row;
	if (num_of_models % num_of_items_per_row != 0) number_of_rows++;
	int calculated_height = number_of_rows * (gridSize().height() + num_of_items_per_row*2);

	setFixedHeight(calculated_height);


	
		int defaultItemSize = 70;
		int columnCount = width() / (gridSize().width() + 10);
		int gridCount = count();
		if (columnCount > numberOfItemPerRow) numberOfItemPerRow++;

		while (width() > defaultItemSize*numberOfItemPerRow) defaultItemSize++;
		while (width() < defaultItemSize*numberOfItemPerRow) defaultItemSize--;

		//setIconSize({ defaultItemSize, defaultItemSize });
		//setGridSize({ defaultItemSize, defaultItemSize });


		emit resizeItem(defaultItemSize);
		qDebug() << defaultItemSize << numberOfItemPerRow;
	
}




QMimeData * ListWidget::mimeData(const QList<QListWidgetItem *> items) const
{

	
	QMimeData *data = new QMimeData();
	//set text for shader
	data->setText(items[0]->data(Qt::DisplayRole).toString());
	//set html for node
	data->setHtml(items[0]->data(Qt::UserRole).toString());
	data->setData("MODEL_TYPE_ROLE", items[0]->data(MODEL_TYPE_ROLE).toByteArray());
	data->setData("MODEL_GUID_ROLE", items[0]->data(MODEL_GUID_ROLE).toByteArray());

	if (!items[0]->data(MODEL_EXT_ROLE).isNull()) data->setData("index", items[0]->data(MODEL_EXT_ROLE).toByteArray());
	return data;
}

void ListWidget::resizeEvent(QResizeEvent * event)
{
	QListWidget::resizeEvent(event);
	if(isResizable) displayAllContents();
}

void ListWidget::addToListWidget(QListWidgetItem * item)
{
	connect(this, &ListWidget::resizeItem, [=](int size) {
	//	item->setSizeHint({ size,size });
	});

	addItem(item);
}

void ListWidget::dropEvent(QDropEvent * event)
{
    QListWidget::dropEvent(event);
}

void ListWidget::createContextMenu()
{

}

void ListWidget::customContextMenu(QPoint pos)
{
    QModelIndex index = indexAt(pos);
    auto guid = index.data(MODEL_GUID_ROLE).toString();

    QMenu menu;
    menu.setStyleSheet(
        "QMenu { background-color: #1A1A1A; color: #EEE; padding: 0; margin: 0; }"
        "QMenu::item { background-color: #1A1A1A; padding: 6px 8px; margin: 0; }"
        "QMenu::item:selected { background-color: #3498db; color: #EEE; padding: 6px 8px; margin: 0; }"
        "QMenu::item : disabled { color: #555; }"
    );


    if(shaderContextMenuAllowed){
        if(index.isValid()){
            auto actionRename = new QAction("Rename");
            auto actionExport = new QAction("Export");
            auto actionEdit = new QAction("Edit");
            auto actionDelete = new QAction("Delete");

            connect(actionRename,&QAction::triggered,[guid ,this](){
                emit renameShader(guid);

            });
            connect(actionExport,&QAction::triggered,[guid ,this](){
                emit exportShader(guid);
            });
            connect(actionEdit,&QAction::triggered,[guid, index ,this](){
               // this->edit(index);
                emit editShader(guid);
            });
            connect(actionDelete,&QAction::triggered,[guid ,this](){
                emit deleteShader(guid);
            });

            menu.addActions({actionRename,actionEdit,actionExport,actionDelete});
            menu.exec(this->mapToGlobal(pos));
        }else{
            auto actionCreate = new QAction("Create Shader");
            auto actionImport = new QAction("Import Shader");

            connect(actionCreate,&QAction::triggered,[guid ,this](){
                emit createShader(guid);
            });
            connect(actionImport,&QAction::triggered,[guid ,this](){
                emit importShader(guid);
            });

            menu.addActions({actionCreate, actionImport});
            menu.exec(this->mapToGlobal(pos));
        }
    }else{

    }
}


