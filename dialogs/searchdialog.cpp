#include "searchdialog.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QTabWidget>
#include "../nodes/libraryv1.h"
#include "../properties.h"
#include "../core/project.h"



SearchDialog::SearchDialog(NodeGraph *graph) : QDialog()
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);
	setAttribute(Qt::WA_TranslucentBackground);
	setAttribute(Qt::WA_NoSystemBackground, true);
	setMouseTracking(true);
	setWindowFlag(Qt::SubWindow);
	setAttribute(Qt::WA_QuitOnClose, false);
	

	auto layout = new QVBoxLayout;
	setLayout(layout);
	setMinimumSize(350, 150);
	setWindowModality(Qt::NonModal);

	auto tabWidget = new QTabWidget;
	auto nodeWidget = new QWidget;
	auto nodeLayout = new QVBoxLayout;

	nodeContainer = new ListWidget;
	propertyContainer = new ListWidget;
    //nodeContainer->setAlternatingRowColors(false);
    //nodeContainer->setSpacing(0);
    //nodeContainer->setContentsMargins(10, 3, 10, 10);
    //nodeContainer->setViewMode(QListWidget::IconMode);
    //nodeContainer->setIconSize({90,90});
    //nodeContainer->setMouseTracking(true);
    //nodeContainer->setDragDropMode(QAbstractItemView::DragDrop);
    //nodeContainer->setMovement(QListView::Static);
    //nodeContainer->setResizeMode(QListWidget::Adjust);
    //nodeContainer->setDefaultDropAction(Qt::CopyAction);
    //nodeContainer->setSelectionMode(QAbstractItemView::SingleSelection);
    //nodeContainer->setSelectionRectVisible(false);
    //nodeContainer->setDragEnabled(true);
    //nodeContainer->viewport()->setAcceptDrops(false);
    //nodeContainer->setDropIndicatorShown(true);
    //nodeContainer->installEventFilter(this);
    //nodeContainer->viewport()->installEventFilter(this);
    //nodeContainer->setWordWrap(true);
    //nodeContainer->setGridSize(QSize(90, 90));
    //nodeContainer->setSortingEnabled(true);
    //nodeContainer->sortItems();
    //nodeContainer->setEditTriggers(QAbstractItemView::NoEditTriggers);

	QFont font;
	font.setPointSizeF(font.pointSize() * devicePixelRatioF());
	setFont(font);

	//search box
	auto searchContainer = new QWidget;
	auto searchLayout = new QHBoxLayout;
	auto searchBar = new QLineEdit;

	searchContainer->setLayout(searchLayout);
	searchLayout->addWidget(searchBar);
	searchLayout->addSpacing(12);

	searchBar->setPlaceholderText("search");
	searchBar->setAlignment(Qt::AlignLeft);
	searchBar->setFont(font);
	searchBar->setTextMargins(8, 0, 0, 0);
	QSize currentSize(90, 90);




	nodeWidget->setLayout(nodeLayout);
	nodeLayout->addWidget(searchContainer);
	nodeLayout->addWidget(nodeContainer);

	tabWidget->addTab(nodeWidget, "Nodes");
	tabWidget->addTab(propertyContainer, "Properties");

	tabWidget->setStyleSheet(
		"QTabWidget::pane{	border: 1px solid rgba(0, 0, 0, .5); border - top: 0px solid rgba(0, 0, 0, 0);}"
		"QTabBar::tab{	background: rgba(21, 21, 21, .7); color: rgba(250, 250, 250, .9); font - weight: 400; font - size: 13em; padding: 5px 22px 5px 22px; }"
		"QTabBar::tab:selected{ color: rgba(255, 255, 255, .99); border-top: 2px solid rgba(50,150,250,.8); }"
		"QTabBar::tab:!selected{ background: rgba(55, 55, 55, .99); border : 1px solid rgba(21,21,21,.4); color: rgba(200,200,200,.5); }"
	);

	layout->addWidget(tabWidget);


//	layout->addWidget(searchContainer);
//	layout->addWidget(nodeContainer);
	generateTileNode(graph);
	generateTileProperty(graph);


	connect(searchBar, &QLineEdit::textChanged, [=](QString str) {
		nodeContainer->clear();
		QList<NodeLibraryItem*> lis;
		for (auto item : graph->library->items) {
			if (item->displayName.contains(str, Qt::CaseInsensitive)) lis.append(item);
		}
		
		generateTileNode(lis);

		if (str.length() == 0) {
			nodeContainer->clear();
			generateTileNode(graph);
		}

	});

	searchContainer->setStyleSheet("background:rgba(32,32,32,0);");
	searchBar->setStyleSheet("QLineEdit{ background:rgba(41,41,41,1); border: 1px solid rgba(150,150,150,.2); border-radius: 1px; color: rgba(250,250,250,.95); }");

	setStyleSheet(""

		"QWidget{background: rgba(21,21,21,1); border: 0px solid rgba(0,0,0,0);}"

		"QListView::item{ border-radius: 2px; border: 1px solid rgba(0,0,0,1); background: rgba(80,80,80,1); margin: 3px; color: rgba(253,253,253,1);  }"
		"QListView::item:selected{ background: rgba(65,65,65,1); border: 1px solid rgba(50,150,250,1); }"
		"QListView::item:hover{ background: rgba(55,55,55,1); border: 1px solid rgba(50,150,250,1); }"
		"QListView::text{ top : -6; }"
	);

}


SearchDialog::~SearchDialog()
{
}

void SearchDialog::generateTileNode(QList<NodeLibraryItem*> lis)
{

	QSize currentSize(90, 90);

	for (auto tile : lis) {

		auto item = new QListWidgetItem;
		item->setText(tile->displayName);
		item->setData(Qt::DisplayRole, tile->displayName);
		item->setData(Qt::UserRole, tile->name);
		item->setData(MODEL_TYPE_ROLE, "node");
		item->setSizeHint(currentSize);
		item->setTextAlignment(Qt::AlignCenter);
		item->setFlags(item->flags() | Qt::ItemIsEditable);
		item->setIcon(QIcon(":/icons/icon.png"));

		nodeContainer->addItem(item);
	}
}

void SearchDialog::generateTileNode(NodeGraph *graph)
{

	QSize currentSize(90, 90);

	for (NodeLibraryItem *tile : graph->library->items) {
		if (tile->name == "property") continue;
		auto item = new QListWidgetItem;
		item->setText(tile->displayName);
		item->setData(Qt::DisplayRole, tile->displayName);
		item->setData(Qt::UserRole, tile->name);
		item->setData(MODEL_TYPE_ROLE, "node");
		item->setSizeHint(currentSize);
		item->setTextAlignment(Qt::AlignCenter);
		item->setFlags(item->flags() | Qt::ItemIsEditable);
		item->setIcon(QIcon(":/icons/icon.png"));
		nodeContainer->addItem(item);
	}
}

void SearchDialog::generateTileProperty(NodeGraph * graph)
{
	QSize currentSize(90, 90);

	for (auto tile : graph->properties) {
		if (tile->name == "property") continue;
		auto item = new QListWidgetItem;
		item->setText(tile->displayName);
		item->setData(Qt::DisplayRole, tile->displayName);
		item->setData(Qt::UserRole, tile->name);
		item->setData(MODEL_EXT_ROLE, index);
		item->setSizeHint(currentSize);
		item->setTextAlignment(Qt::AlignCenter);
		item->setFlags(item->flags() | Qt::ItemIsEditable);
		item->setIcon(QIcon(":/icons/icon.png"));
		propertyContainer->addItem(item);
		index++;
	}

}

void SearchDialog::leaveEvent(QEvent * event)
{
	this->close();
    this->deleteLater();
}
