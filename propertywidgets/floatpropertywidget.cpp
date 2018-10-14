#include "floatpropertywidget.h"
#include <QLabel>

FloatPropertyWidget::FloatPropertyWidget() : BasePropertyWidget()
{
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	auto mainLayout = layout;
	
	auto wid = new WidgetFloat;
	floatSpinBox = wid->floatSpinBox;
	minSpinBox = wid->minSpinBox;
	maxSpinBox = wid->maxSpinBox;
	stepSpinBox = wid->stepSpinBox;


	setConnections();
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
	floatSpinBox->setValue(prop->value);
	minSpinBox->setValue(prop->minValue);
	maxSpinBox->setValue(prop->maxValue);
	stepSpinBox->setValue(prop->step);
	modelProperty = prop;
	emit nameChanged(displayName->text());

}

float FloatPropertyWidget::getValue()
{
	return x;
}

void FloatPropertyWidget::setPropValue(double value)
{
	prop->setValue(value);
}


void FloatPropertyWidget::setConnections() {
	connect(floatSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double val) {
		x = val;
		emit valueChanged(x);
	});
	connect(minSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double val) {
		floatSpinBox->setMinimum(val);
	});
	connect(maxSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double val) {
		floatSpinBox->setMaximum(val);

	});
	connect(stepSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double val) {
		floatSpinBox->setSingleStep(val);

	});

	connect(this, &FloatPropertyWidget::valueChanged, [=](double val) {
		setPropValue(val);
	});
}



