#include "floatpropertywidget.h"
#include <QLabel>

FloatPropertyWidget::FloatPropertyWidget()
{
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	auto mainLayout = new QVBoxLayout();
	auto textLayout = new QHBoxLayout();

	displayName = new QLineEdit(this);
	auto label = new QLabel("Display Name", this);

	textLayout->addWidget(label);
	textLayout->addStretch();
	textLayout->addWidget(displayName);
	auto wid = getValueWidget();

	mainLayout->addLayout(textLayout);
	mainLayout->addWidget(wid);
	setLayout(mainLayout);
}

FloatPropertyWidget::~FloatPropertyWidget()
{
}

void FloatPropertyWidget::setProp(FloatProperty * prop)
{
	this->prop = prop;
	displayName->setText(prop->displayName);
	intSpinBox->setValue(prop->value);
	minSpinBox->setValue(prop->minValue);
	maxSpinBox->setValue(prop->maxValue);
	stepSpinBox->setValue(prop->step);
}

float FloatPropertyWidget::getValue()
{
	return x;
}

void FloatPropertyWidget::setPropValue(double value)
{
	prop->setValue(value);
}

QWidget * FloatPropertyWidget::getValueWidget()
{
	widget = new QWidget(this);
	auto mainLayout = new QVBoxLayout;
	auto spinLayout = new QHBoxLayout();
	auto label = new QLabel("Values", this);

	spinLayout->setContentsMargins(0, 0, 0, 0);


	widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	widget->setLayout(mainLayout);

	intSpinBox = new QDoubleSpinBox(this);
	maxSpinBox = new QDoubleSpinBox(this);
	minSpinBox = new QDoubleSpinBox(this);
	stepSpinBox= new QDoubleSpinBox(this);

	auto gridLayout = new QGridLayout;
	gridLayout->addWidget(minSpinBox, 0, 1);
	gridLayout->addWidget(maxSpinBox, 1, 1);
	gridLayout->addWidget(stepSpinBox, 2,1);
	gridLayout->addWidget(new QLabel("min"), 0, 0);
	gridLayout->addWidget(new QLabel("max"), 1, 0);
	gridLayout->addWidget(new QLabel("step"), 2, 0);
	
	spinLayout->addWidget(label);
	spinLayout->addWidget(intSpinBox);

	mainLayout->addLayout(spinLayout);
	mainLayout->addLayout(gridLayout);

	connect(intSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double val) {
		x = val;
		emit valueChanged(x);
	});
	connect(minSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double val) {
		intSpinBox->setMinimum(val);
	});
	connect(maxSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double val) {
		intSpinBox->setMaximum(val);

	});
	connect(stepSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double val) {
		intSpinBox->setSingleStep(val);

	});


	connect(this, &FloatPropertyWidget::valueChanged, [=](double val) {
		setPropValue(val);
	});
	return widget;
}

QWidget * FloatPropertyWidget::getWidget()
{
	if (widget) return widget;
	return nullptr;
}

void FloatPropertyWidget::paintEvent(QPaintEvent * event)
{
	QWidget::paintEvent(event);
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(QPen(QColor(200, 200, 200, 70), 2));
	painter.drawRect(0, 0, width(), height());
}
