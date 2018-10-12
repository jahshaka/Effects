#include "basepropertywidget.h"
#include <QPushButton>
#include <QDebug>
#include <QResizeEvent>
#include <QApplication>
#include <QAction>
#include <QPainter>


BasePropertyWidget::BasePropertyWidget(QWidget * parent) : QWidget(parent)
{
	displayName = new QLineEdit;
	displayName->setText("Display");
	displayName->setAlignment(Qt::AlignHCenter);

	button = new QPushButton;
	button->setMaximumSize(12, 12);
	button->setIcon(QIcon(":/images/delete-26.png"));
	button->setIconSize(QSize(12, 12));




	displayWidget = new QWidget;
	displayWidget->setStyleSheet("background: rgba(0,0,0,0);");
	auto displayLayout = new QHBoxLayout;
	displayWidget->setLayout(displayLayout);
	displayLayout->addStretch();
	displayLayout->addWidget(displayName);
	displayLayout->addStretch();
	displayLayout->addWidget(button);
	displayLayout->setContentsMargins(5, 5, 2, 5);


	layout = new QVBoxLayout;
	layout->setContentsMargins(5, 0, 5, 5);
	layout->addWidget(displayWidget);
	layout->addSpacing(7);
	setLayout(layout);

	connect(button, &QPushButton::clicked, [=]() {
		emit buttonPressed();
	});

	setStyleSheet("QMenu{	background: rgba(26,26,26,.9); color: rgba(250,250, 250,.9);}"
		"QMenu::item{padding: 2px 5px 2px 20px;	}"
		"QMenu::item:hover{	background: rgba(40,128, 185,.9);}"
		"QMenu::item:selected{	background: rgba(40,128, 185,.9);}"
	);
}


BasePropertyWidget::~BasePropertyWidget()
{
}


void BasePropertyWidget::paintEvent(QPaintEvent * event)
{
	QWidget::paintEvent(event);

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setRenderHint(QPainter::HighQualityAntialiasing);


	// draw title border
	QPainterPath path;
	path.addRoundedRect(0, 0, width(), displayWidget->height(), 5, 5);
	painter.fillPath(path, QColor(22, 22, 22));

	//draw outline
	painter.setPen(QPen(QColor(70, 70, 70), 2));
	painter.drawRoundedRect(0, 0, width(), height(),5,5);



}
