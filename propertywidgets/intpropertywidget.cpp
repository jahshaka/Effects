#include "intpropertywidget.h"
#include <QLabel>

IntPropertyWidget::IntPropertyWidget() : BasePropertyWidget()
{
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	auto mainLayout = layout;

	wid = new WidgetInt;
	setWidget(wid);
	intSpinBox = wid->spinBox;
	minSpinBox = wid->minSpinBox;
	maxSpinBox = wid->maxSpinBox;
	stepSpinBox = wid->stepSpinBox;
	setConnections();

	mainLayout->addWidget(wid);
}

IntPropertyWidget::~IntPropertyWidget()
{
}

void IntPropertyWidget::setProp(IntProperty * prop)
{
	this->prop = prop;
	displayName->setText(prop->displayName);
	intSpinBox->setValue(prop->value);
	minSpinBox->setValue(prop->minValue);
	maxSpinBox->setValue(prop->maxValue);
	stepSpinBox->setValue(prop->step);
	modelProperty = prop;
	emit nameChanged(displayName->text());

}

int IntPropertyWidget::getValue()
{
	return x;
}

void IntPropertyWidget::setPropValue(int value)
{
	prop->setValue(value);
}


void IntPropertyWidget::setConnections() {
	connect(intSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int val) {
		x = val;
		emit valueChanged(x);
	});
	connect(minSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int val) {
		intSpinBox->setMinimum(val);
	});
	connect(maxSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int val) {
		intSpinBox->setMaximum(val);

	});
	connect(stepSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int val) {
		intSpinBox->setSingleStep(val);
	});


	connect(this, &IntPropertyWidget::valueChanged, [=](int val) {
		setPropValue(val);
	});

	connect(this, &BasePropertyWidget::shouldSetVisible, [=](bool val) {
		wid->setVisible(val);
	});
}

