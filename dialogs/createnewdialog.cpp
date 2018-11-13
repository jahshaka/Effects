#include "createnewdialog.h"
#include <QLayout>
#include "../mainwindow.h"

CreateNewDialog::CreateNewDialog(QList<nodeGraphPreset> list) : QDialog()
{

	auto layout = new QVBoxLayout;
	setLayout(layout);
	setMinimumSize(550,250);


	QSize currentSize(90, 90);

	options = new QWidget;
	presets = new QWidget;
	nameEdit = new QLineEdit;

	optionWidget = new ListWidget;
	presetWidget = new ListWidget;
	optionsScroll = new QScrollArea;
	auto scrollHolder = new QWidget;
	auto scrollLayout = new QGridLayout;
	tabbedWidget = new QTabWidget;

	cancel = new QPushButton("Cancel");
	confirm = new QPushButton("Confirm");

	auto buttonHolder = new QWidget;
	auto buttonLayout = new QHBoxLayout;
	buttonHolder->setLayout(buttonLayout);
	buttonLayout->addStretch();
	buttonLayout->addWidget(cancel);
	buttonLayout->addWidget(confirm);

	auto nameHolder = new QWidget;
	auto nameHolderLayout = new QHBoxLayout;
	nameHolder->setLayout(nameHolderLayout);
	nameHolderLayout->addWidget(new QLabel("Name:"));
	nameHolderLayout->addWidget(nameEdit);

	holder = new QWidget;
	auto holderLayout = new QVBoxLayout;
	holder->setLayout(holderLayout);
	holderLayout->setContentsMargins(0, 20, 0, 0);
	holderLayout->addWidget(tabbedWidget);

	

	layout->addWidget(holder);
	layout->addSpacing(15);
	layout->addWidget(nameHolder);
	layout->addWidget(buttonHolder);

	auto optionLayout = new QVBoxLayout;
	auto presetLayout = new QVBoxLayout;

	options->setLayout(optionLayout);
	presets->setLayout(presetLayout);

	optionLayout->addWidget(optionWidget);
	presetLayout->addWidget(presetWidget);

	nameEdit->setPlaceholderText("Enter name here...");

	scrollHolder->setLayout(scrollLayout);
	optionsScroll->setWidget(scrollHolder);

	tabbedWidget->addTab(options, "New");
	tabbedWidget->addTab(presets, "Presets");
	tabbedWidget->addTab(scrollHolder, "next");

	int i = 0;
	int j = 0;

	//set up list widgets
	for (auto tile : list) {
		auto item = new OptionSelection(tile);
		item->setText(tile.name);
		scrollLayout->addWidget(item,i,j);
		j++;
		if (j % 5 == 0) {
			j = 0;
			i++;
		}
	}


	//set up list widgets
	for (auto tile : list) {
		auto item = new QListWidgetItem;
		item->setText(tile.name);
		item->setData(Qt::DisplayRole, tile.title);
		item->setData(Qt::UserRole, tile.name);
		item->setSizeHint(currentSize);
		item->setTextAlignment(Qt::AlignCenter);
		item->setFlags(item->flags() | Qt::ItemIsEditable /*| Qt::ItemIsUserCheckable*/);
		item->setIcon(QIcon(":/icons/icon.png"));
		item->setBackgroundColor(QColor(60, 60, 60));
		//item->setCheckState(Qt::Unchecked);
		//optionWidget->addItem(item);
	}

	//set up new presets
	for (auto tile : list) {
		// should grab presets from somehwere?
		auto item = new QListWidgetItem;
		item->setText(tile.name);
		item->setData(Qt::DisplayRole, tile.title);
		item->setData(Qt::UserRole, tile.name);
		item->setSizeHint(currentSize);
		item->setTextAlignment(Qt::AlignCenter);
		item->setFlags(item->flags() | Qt::ItemIsEditable /*| Qt::ItemIsUserCheckable*/);
		item->setIcon(QIcon(":/icons/icon.png"));
		item->setBackgroundColor(QColor(60, 60, 60));
		//item->setCheckState(Qt::Unchecked);
		//presetWidget->addItem(item);

	//	connect(item, &QListWidgetItem::)
	}

	connect(optionWidget, &ListWidget::itemSelectionChanged, [=]() {
		if (optionWidget->selectedItems().count() > 0)	presetWidget->setEnabled(false);
		else 	presetWidget->setEnabled(true);
	});

	connect(presetWidget, &ListWidget::itemSelectionChanged, [=]() {
		if (presetWidget->selectedItems().count() > 0)	optionWidget->setEnabled(false);
		else 	optionWidget->setEnabled(true);
	});

	connect(cancel, &QPushButton::clicked, [=]() {
		this->reject();
	});
	connect(cancel, &QPushButton::clicked, [=]() {

		auto projectName = nameEdit->text();
		auto optionSelected = optionWidget->selectedItems().at(0);


		this->accept();
	});

	//configureStylesheet();
}


CreateNewDialog::~CreateNewDialog()
{
}

void CreateNewDialog::configureStylesheet()
{
	setStyleSheet(
		"QListView::item{ border-radius: 2px; border: 1px solid rgba(0,0,0,1); background: rgba(80,80,80,1); margin: 3px;  }"
		"QListView::item:selected{ background: rgba(65,65,65,1); border: 1px solid rgba(50,150,250,1); }"
		"QListView::item:hover{ background: rgba(55,55,55,1); border: 1px solid rgba(50,150,250,1); }"
		"QListView::text{ top : -6; }"

		"QWidget{background:rgba(32,32,32,1); color:rgba(240,240,240,1); border: 2px solid rgba(0,0,0,0);}"
		"QMenu{	background: rgba(26,26,26,.9); color: rgba(250,250, 250,.9); border-radius : 2px; }"
		"QMenu::item{padding: 4px 5px 4px 10px;	}"
		"QMenu::item:hover{	background: rgba(40,128, 185,.9);}"
		"QMenu::item:selected{	background: rgba(40,128, 185,.9);}"

		"QScrollBar:vertical {border : 0px solid black;	background: rgba(132, 132, 132, 0);width: 24px; padding: 4px;}"
		"QScrollBar::handle{ background: rgba(72, 72, 72, 1);	border-radius: 8px; width: 14px; }"
		"QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {	background: rgba(200, 200, 200, 0);}"
		"QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical {	background: rgba(0, 0, 0, 0);border: 0px solid white;}"
		"QScrollBar::sub-line, QScrollBar::add-line {	background: rgba(10, 0, 0, .0);}"

		//"QPushButton{ background: #333; color: #DEDEDE; border : 0; padding: 4px 16px; }"
		"QPushButton:hover{ background-color: #555; }"
		"QPushButton:pressed{ background-color: #444; }"
		"QPushButton:disabled{ color: #444; }"
		"QPushButton:checked{ background-color: rgba(50,150,255,1); }"
	);

	holder->setStyleSheet(
//		"QMainWindow::separator {width: 10px;h eight: 0px; margin: -3.5px; padding: 0px; border: 0px solid black; background: rgba(19, 19, 19, 1);}"
		"QWidget{background:rgba(32,32,32,1); color:rgba(240,240,240,1); border: 0px solid rgba(0,0,0,0);}"
		"QMenu{	background: rgba(26,26,26,.9); color: rgba(250,250, 250,.9); border-radius : 2px; }"
		"QMenu::item{padding: 4px 5px 4px 10px;	}"
		"QMenu::item:hover{	background: rgba(40,128, 185,.9);}"
		"QMenu::item:selected{	background: rgba(40,128, 185,.9);}"

		"QTabWidget::pane{border: 1px solid rgba(0,0,0,.5);	border - top: 0px solid rgba(0,0,0,0);	}"
		"QTabWidget::tab - bar{	left: 1px;	}"
		"QDockWidget::tab{	background:rgba(32,32,32,1);}"

		"QScrollBar:vertical, QScrollBar:horizontal {border : 0px solid black;	background: rgba(132, 132, 132, 0);width: 24px; padding: 4px;}"
		"QScrollBar::handle{ background: rgba(72, 72, 72, 1);	border-radius: 8px; width: 14px; }"
		"QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {	background: rgba(200, 200, 200, 0);}"
		"QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical {	background: rgba(0, 0, 0, 0);border: 0px solid white;}"
		"QScrollBar::sub-line, QScrollBar::add-line {	background: rgba(10, 0, 0, .0);}"
	);

	tabbedWidget->setStyleSheet(
		"QTabWidget::pane{	border: 1px solid rgba(0, 0, 0, .5); border - top: 0px solid rgba(0, 0, 0, 0);}"
		"QTabBar::tab{	background: rgba(21, 21, 21, .7); color: rgba(250, 250, 250, .9); font - weight: 400; font - size: 13em; padding: 5px 22px 5px 22px; }"
		"QTabBar::tab:selected{ color: rgba(255, 255, 255, .99); border-top: 2px solid rgba(50,150,250,.8); }"
		"QTabBar::tab:!selected{ background: rgba(55, 55, 55, .99); border : 1px solid rgba(21,21,21,.4); color: rgba(200,200,200,.5); }"
	);
	
}

void CreateNewDialog::configureListViews()
{
	QSize currentSize(90, 90);

	optionWidget->setAlternatingRowColors(false);
	optionWidget->setSpacing(0);
	optionWidget->setContentsMargins(10, 3, 10, 10);
	optionWidget->setViewMode(QListWidget::IconMode);
	optionWidget->setIconSize(currentSize);
	optionWidget->setMouseTracking(true);
	optionWidget->setDragDropMode(QAbstractItemView::DragDrop);
	optionWidget->setMovement(QListView::Static);
	optionWidget->setResizeMode(QListWidget::Adjust);
	optionWidget->setDefaultDropAction(Qt::MoveAction);
	optionWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	optionWidget->setSelectionRectVisible(false);
	optionWidget->setDragEnabled(true);
	optionWidget->viewport()->setAcceptDrops(false);
	optionWidget->setDropIndicatorShown(true);
	optionWidget->installEventFilter(this);
	optionWidget->viewport()->installEventFilter(this);
	optionWidget->setWordWrap(true);
	optionWidget->setGridSize(QSize(100, 100));
	optionWidget->setSortingEnabled(true);
	optionWidget->sortItems();
	optionWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);


	presetWidget->setAlternatingRowColors(false);
	presetWidget->setSpacing(0);
	presetWidget->setContentsMargins(10, 3, 10, 10);
	presetWidget->setViewMode(QListWidget::IconMode);
	presetWidget->setIconSize(currentSize);
	presetWidget->setMouseTracking(true);
	presetWidget->setDragDropMode(QAbstractItemView::DragDrop);
	presetWidget->setMovement(QListView::Static);
	presetWidget->setResizeMode(QListWidget::Adjust);
	presetWidget->setDefaultDropAction(Qt::MoveAction);
	presetWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	presetWidget->setSelectionRectVisible(false);
	presetWidget->setDragEnabled(true);
	presetWidget->viewport()->setAcceptDrops(false);
	presetWidget->setDropIndicatorShown(true);
	presetWidget->installEventFilter(this);
	presetWidget->viewport()->installEventFilter(this);
	presetWidget->setWordWrap(true);
	presetWidget->setGridSize(QSize(90, 90));
	presetWidget->setSortingEnabled(true);
	presetWidget->sortItems();
	presetWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}


OptionSelection::OptionSelection(nodeGraphPreset node) : QPushButton()
{
	setFixedSize(120, 120);

	if (info.iconPath == "") setIcon(QIcon(info.iconPath));
	else setIcon(QIcon(":/icons/icon.png"));
	setIconSize(QSize(120,120));
	setCheckable(true);
	setAutoExclusive(true);

	auto layout = new QVBoxLayout;
//	setLayout(layout);

	
	

}
