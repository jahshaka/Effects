#include "createnewdialog.h"
#include <QLayout>
#include <QPainter>
#include "../mainwindow.h"
#include <QDebug>

CreateNewDialog::CreateNewDialog(QList<nodeGraphPreset> list) : QDialog()
{

	auto layout = new QVBoxLayout;
	setLayout(layout);
	setMinimumSize(480,250);

	QSize currentSize(90, 90);

	nameEdit = new QLineEdit;

	optionsScroll = new QScrollArea;
    presetsScroll = new QScrollArea;
    options = new QWidget;
    presets = new QWidget;
    auto optionLayout = new QGridLayout;
    auto presetLayout = new QGridLayout;
	infoLabel = new QLabel;

    //controls pading in selection window
    optionLayout->setContentsMargins(10,20,15,25);
    presetLayout->setContentsMargins(10,20,15,25);

    tabbedWidget = new QTabWidget;
    cancel = new QPushButton("Cancel");
    confirm = new QPushButton("Confirm");
	confirm->setDefault(true);
	confirm->setEnabled(false);

    options->setLayout(optionLayout);
    presets->setLayout(presetLayout);
	optionLayout->setSpacing(20);
	presetLayout->setSpacing(20);

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
	nameHolderLayout->addSpacing(5);
	nameHolderLayout->addWidget(nameEdit);

    holder = new QWidget;
	auto holderLayout = new QVBoxLayout;
	holder->setLayout(holderLayout);
	holderLayout->setContentsMargins(0, 20, 0, 0);
	holderLayout->addWidget(tabbedWidget);

    optionsScroll->setWidget(options);
    presetsScroll->setWidget(presets);
    optionsScroll->setWidgetResizable(true);
    presetsScroll->setWidgetResizable(true);


	layout->addWidget(holder);
	layout->addSpacing(15);
	layout->addWidget(infoLabel);
	layout->addWidget(nameHolder);
	layout->addWidget(buttonHolder);

	nameEdit->setPlaceholderText("Enter name here...");
	nameEdit->setTextMargins(3, 0, 0, 0);
	infoLabel->setAlignment(Qt::AlignCenter);
	infoLabel->setObjectName(QStringLiteral("infoLabel"));

    tabbedWidget->addTab(optionsScroll, "options");
    tabbedWidget->addTab(presetsScroll, "presets");

	int i = 0;
	int j = 0;


    //set up list options
    for (auto tile : list) {
        auto item = new OptionSelection(tile);
        optionLayout->addWidget(item,i,j);
        j++;
        if (j % num_of_widgets_per_row == 0) {
            j = 0;
            i++;
        }
		connect(item, &OptionSelection::buttonSelected, [=](OptionSelection* button) {
			currentInfoSelected = button->info;
			infoLabel->setText(currentInfoSelected.title + " selected");
			templateName = button->info.name;
			type = 1;
			confirm->setEnabled(true);
		});
    }
	

    i=0;j=0;

    //set up list for presets from whereveer ?
    for (auto tile : list) {
        auto item = new OptionSelection(tile);
        presetLayout->addWidget(item,i,j);
        j++;
        if (j % num_of_widgets_per_row == 0) {
            j = 0;
            i++;
        }
		connect(item, &OptionSelection::buttonSelected, [=](OptionSelection* button) {
			currentInfoSelected = button->info;
			templateName = button->info.name;
			type = 2;
			infoLabel->setText(currentInfoSelected.title + " selected");
		});
    }


	connect(cancel, &QPushButton::clicked, [=]() {
		this->reject();
	});
	connect(confirm, &QPushButton::clicked, [=]() {
		auto projectName = nameEdit->text();
		this->accept();
		emit confirmClicked(2);
	});

	connect(nameEdit, &QLineEdit::textChanged, [=](QString text) {
		if(text.count() > 1) 	confirm->setEnabled(true);
		else 	confirm->setEnabled(false);

		name = text;

	});

    configureStylesheet();
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

        "QScrollBar:vertical, QScrollBar:horizontal {border : 0px solid black;	background: rgba(132, 132, 132, 0);width: 18px; padding: 4px;}"
        "QScrollBar::handle{ background: rgba(72, 72, 72, 1);	border-radius: 4px; width: 8px; }"
        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {	background: rgba(200, 200, 200, 0);}"
        "QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical {	background: rgba(0, 0, 0, 0);border: 0px solid white;}"
        "QScrollBar::sub-line, QScrollBar::add-line {	background: rgba(10, 0, 0, .0);}"

        "QWidget{background:rgba(32,32,32,1); color:rgba(240,240,240,1); border: 0px solid rgba(0,0,0,0);}"
		"QMenu{	background: rgba(26,26,26,.9); color: rgba(250,250, 250,.9); border-radius : 2px; }"
		"QMenu::item{padding: 4px 5px 4px 10px;	}"
		"QMenu::item:hover{	background: rgba(40,128, 185,.9);}"
		"QMenu::item:selected{	background: rgba(40,128, 185,.9);}"

        "QPushButton{ background: #333; color: #DEDEDE; border : 0; padding: 4px 16px; }"
		"QPushButton:hover{ background-color: #555; }"
		"QPushButton:pressed{ background-color: #444; }"
		"QPushButton:disabled{ color: #444; }"
		"QPushButton:checked{ background-color: rgba(50,150,255,1); }"

		"QLineEdit{background: rgba(0,0,0,0); border-bottom: 1px solid rgba(50,50,50,1);}"
		"QLabel#infoLabel{color: rgba(200,200,200,.5);}"
	);

	holder->setStyleSheet(
		"QWidget{background:rgba(32,32,32,1); color:rgba(240,240,240,1); border: 0px solid rgba(0,0,0,0);}"
		"QMenu{	background: rgba(26,26,26,.9); color: rgba(250,250, 250,.9); border-radius : 2px; }"
		"QMenu::item{padding: 4px 5px 4px 10px;	}"
		"QMenu::item:hover{	background: rgba(40,128, 185,.9);}"
		"QMenu::item:selected{	background: rgba(40,128, 185,.9);}"

		"QTabWidget::pane{border: 0px solid rgba(0,0,0,.5);	border - top: 0px solid rgba(0,0,0,0); border-left : 0px; border-right: 0px;	}"
		"QTabWidget::tab - bar{	left: 1px;	}"
		"QDockWidget::tab{	background:rgba(32,32,32,1);} border: 0px solid rgba(0,0,0,0);"

        "QPushButton{ background: #777; color: #DEDEDE; border : 0; padding: 4px 16px; }"
        "QPushButton:hover{ background-color: #555; }"
        "QPushButton:pressed{ background-color: #444; }"
        "QPushButton:disabled{ color: #444; }"
        "QPushButton:checked{ background-color: rgba(50,150,255,1); }"
	);

	tabbedWidget->setStyleSheet(
		"QTabWidget::pane{	border: 0px solid rgba(0, 0, 0, .5); border-top: 1px solid rgba(0, 0, 0, .4); border-bottom: 1px solid rgba(0,0,0,.4);}"
		"QTabBar::tab{	background: rgba(21, 21, 21, .7); color: rgba(250, 250, 250, .9); font - weight: 400; font - size: 13em; padding: 5px 22px 5px 22px; }"
		"QTabBar::tab:selected{ color: rgba(255, 255, 255, .99); border-top: 2px solid rgba(50,150,250,.8); }"
		"QTabBar::tab:!selected{ background: rgba(55, 55, 55, .99); border : 1px solid rgba(21,21,21,.4); color: rgba(200,200,200,.5); }"
	);
	
}



OptionSelection::OptionSelection(nodeGraphPreset node) : QPushButton()
{
	setFixedSize(120, 120);
	checkedIconIcon.load(":/icons/checked.svg"); 
	checkedIconIcon = checkedIconIcon.scaled(24, 24, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	info = node;

	if (info.iconPath == "") setIcon(QIcon(info.iconPath));
	else setIcon(QIcon(":/icons/icon.png"));
	setIconSize(QSize(120,120));
	setCheckable(true);
	setAutoExclusive(true);

	auto layout = new QVBoxLayout;
    setLayout(layout);
    auto label = new QLabel;
    auto name = new QLabel;
    name->setAlignment(Qt::AlignCenter);
    name->setText(node.title);
    label->setPixmap(QPixmap(":/icons/icon.png"));
    label->setAlignment(Qt::AlignCenter);

    layout->addWidget(label);
    layout->addWidget(name);


    setStyleSheet("QPushButton{ background: #333; color: #DEDEDE; border : 0px; padding: 4px 16px; border-radius: 3px;}"
                  "QPushButton:hover{ background-color: #555; }"
                  "QPushButton:pressed{ background-color: #444; }"
                  "QPushButton:disabled{ color: #444; }"
                  "QPushButton:checked{ background-color: rgba(50,150,250,.8); }"
                  "QLabel{ border: 0; background: rgba(0,0,0,0); }"
                  );

	connect(this, &OptionSelection::clicked, [=]() {
		emit buttonSelected(this);
	});
}

void OptionSelection::paintEvent(QPaintEvent *event)
{
    QPushButton::paintEvent(event);
   
    
	if (isChecked()) {
		QPainter painter(this);
		painter.setRenderHint(QPainter::Antialiasing);
		painter.setRenderHint(QPainter::HighQualityAntialiasing);
		painter.drawPixmap(width() - 25, height() - 25, 23, 23, checkedIconIcon);
	}

}
