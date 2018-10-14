#include "basepropertywidget.h"
#include <QPushButton>
#include <QDebug>
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
	displayLayout->setContentsMargins(5, 7, 2, 7);


	layout = new QVBoxLayout;
	auto mainLayout = new QVBoxLayout;
	mainLayout->addWidget(displayWidget);
	mainLayout->addLayout(layout);
	mainLayout->addSpacing(0);
	mainLayout->setContentsMargins(5, 0, 5, 5);

	setLayout(mainLayout);

	connect(button, &QPushButton::clicked, [=]() {
		emit buttonPressed();
	});

	setStyleSheet("QMenu{	background: rgba(26,26,26,.9); color: rgba(250,250, 250,.9);}"
		"QMenu::item{padding: 2px 5px 2px 20px;	}"
		"QMenu::item:hover{	background: rgba(40,128, 185,.9);}"
		"QMenu::item:selected{	background: rgba(40,128, 185,.9);}"
	);

	auto visibilityBtn = new QPushButton;
	visibilityBtn->setText(tr("minimize"));
	visibilityBtn->setCursor(Qt::PointingHandCursor);
	connect(visibilityBtn, &QPushButton::clicked, [=]() {
		if (minimized) {
			// maximize
			emit shouldSetVisible(minimized);
			visibilityBtn->setText(tr("minimize"));
			minimized = !minimized;
		}
		else {
			 // minimize
			emit shouldSetVisible(minimized);
			visibilityBtn->setText(tr("maximize"));
			minimized = !minimized;

		}
	});

	mainLayout->addWidget(visibilityBtn);

	visibilityBtn->setStyleSheet(""
		"QPushButton{ background: rgba(23,23,23,1); border: .5px solid rgba(0,0,0,1); }"
		"QPushButton:hover{  border: .5px solid rgba(50,150,250,.2); }"
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

void BasePropertyWidget::mouseMoveEvent(QMouseEvent * event)
{
	pressed = true;
	QWidget::mouseMoveEvent(event);
}

void BasePropertyWidget::mousePressEvent(QMouseEvent * event)
{
	emit currentWidget(this);
	QWidget::mousePressEvent(event);
}

void BasePropertyWidget::mouseReleaseEvent(QMouseEvent * event)
{
	pressed = false;
	QWidget::mouseReleaseEvent(event);
}
