#include "intpropertywidget.h"
#include <QLabel>

IntPropertyWidget::IntPropertyWidget() : BasePropertyWidget()
{
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	wid = new WidgetInt;
	setWidget(wid);
	setConnections();

}

IntPropertyWidget
::~IntPropertyWidget()
{
}

void IntPropertyWidget::setProp(IntProperty * prop)
{
	this->prop = prop;
	displayName->setText(prop->displayName);
	wid->spinBox->setValue(prop->value);
	wid->minSpinBox->setValue(prop->minValue);
	wid->maxSpinBox->setValue(prop->maxValue);
	wid->stepSpinBox->setValue(prop->step);
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
	
	connect(this, &IntPropertyWidget::valueChanged, [=](int val) {
		x = val;
		setPropValue(val);
		emit valueChanged(val);
	});
}

