#include "floatpropertywidget.h"
#include <QLabel>

FloatPropertyWidget::FloatPropertyWidget() : BasePropertyWidget()
{
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	wid = new WidgetFloat;
	setWidget(wid);
	setConnections();
}

FloatPropertyWidget::~FloatPropertyWidget()
{
}

void FloatPropertyWidget::setProp(FloatProperty * prop)
{
	this->prop = prop;
	displayName->setText(prop->displayName);
	wid->floatSpinBox->setValue(prop->value);
	wid->minSpinBox->setValue(prop->minValue);
	wid->maxSpinBox->setValue(prop->maxValue);
	wid->stepSpinBox->setValue(prop->step);
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

	connect(this, &FloatPropertyWidget::valueChanged, [=](double val) {
		x = val;
		setPropValue(val);
		emit valueChanged(val);
	});

}



