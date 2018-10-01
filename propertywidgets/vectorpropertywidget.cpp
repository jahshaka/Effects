#include "vectorpropertywidget.h"
#include <QLabel>


VectorPropertyWidget::VectorPropertyWidget()
{
}


VectorPropertyWidget::~VectorPropertyWidget()
{
}


Vector2DPropertyWidget::Vector2DPropertyWidget() : QWidget()
{
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	auto mainLayout = new QVBoxLayout();
	auto textLayout = new QHBoxLayout();

	displayName = new QLineEdit(this);
	auto label = new QLabel( "Display Name", this);

	textLayout->addWidget(label);
	textLayout->addWidget(displayName);
	auto wid = getValueWidget();

	mainLayout->addLayout(textLayout);
	mainLayout->addWidget(wid);
	setLayout(mainLayout);
}


Vector2DPropertyWidget::~Vector2DPropertyWidget()
{
}

void Vector2DPropertyWidget::setProp(Vec2Property *prop)
{
	this->prop = prop;
	displayName->setText(prop->displayName);
	xSpinBox->setValue(prop->value.x());
	ySpinBox->setValue(prop->value.y());
}

QWidget * Vector2DPropertyWidget::getValueWidget()
{
	auto widget = new QWidget(this);
	auto spinLayout = new QHBoxLayout();
	auto label = new QLabel("Values", this);

	spinLayout->setContentsMargins(0, 0, 0, 0);
	

	widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	widget->setLayout(spinLayout);

	xSpinBox = new QSpinBox(this);
	ySpinBox = new QSpinBox(this);

	spinLayout->addWidget(label);
	spinLayout->addWidget(xSpinBox);
	spinLayout->addWidget(ySpinBox);

	connect(xSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int val) {
		x = val;
		emit valueChanged(QVector2D(x, y));
	});
	connect(ySpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int val) {
		y = val;
		emit valueChanged(QVector2D(x, y));
	});

	connect(this, &Vector2DPropertyWidget::valueChanged, [=](QVector2D val) {
		setPropValues(val);
	});


	return widget;
}

void Vector2DPropertyWidget::setPropValues(QVector2D values) {
	x = values.x();
	y = values.y();
	value = values;
}

void Vector2DPropertyWidget::paintEvent(QPaintEvent * event)
{
	QWidget::paintEvent(event);
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(QPen(QColor(200, 200, 200, 70), 2));
	painter.drawRect(0, 0, width(), height());
}
