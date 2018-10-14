#include "vectorpropertywidget.h"
#include <QLabel>
#include <QDebug>

VectorPropertyWidget::VectorPropertyWidget()
{
}


VectorPropertyWidget::~VectorPropertyWidget()
{
}


Vector2DPropertyWidget::Vector2DPropertyWidget() : BasePropertyWidget()
{
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	auto mainLayout = layout;
	auto wid = new Widget2D();
	xSpinBox = wid->xSpinBox;
	ySpinBox = wid->ySpinBox;
	setConnections();
	mainLayout->addWidget(wid);
	setLayout(mainLayout);

}

void Vector2DPropertyWidget::setConnections()
{
	connect(xSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double val) {
		x = val;
		emit valueChanged(QVector2D(x, y));
	});
	connect(ySpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double val) {
		y = val;
		emit valueChanged(QVector2D(x, y));
	});

	connect(this, &Vector2DPropertyWidget::valueChanged, [=](QVector2D val) {
		setPropValues(val);
	});


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
	modelProperty = prop;
	emit nameChanged(displayName->text());

}

void Vector2DPropertyWidget::setPropValues(QVector2D values) {
	x = values.x();
	y = values.y();
	value = values;

	prop->value.setX(x);
	prop->value.setY(y);
}




/////////////////////////////




Vector3DPropertyWidget::Vector3DPropertyWidget() : BasePropertyWidget()
{
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	auto mainLayout = layout;
	
	auto wid = new Widget3D;
	xSpinBox = wid->xSpinBox;
	ySpinBox = wid->ySpinBox;
	zSpinBox = wid->zSpinBox;	
	setConnections();
	mainLayout->addWidget(wid);
	setLayout(mainLayout);

}


Vector3DPropertyWidget::~Vector3DPropertyWidget()
{
}

void Vector3DPropertyWidget::setProp(Vec3Property *prop)
{
	this->prop = prop;
	displayName->setText(prop->displayName);
	xSpinBox->setValue(prop->value.x());
	ySpinBox->setValue(prop->value.y());
	zSpinBox->setValue(prop->value.z());
	modelProperty = prop;
	emit nameChanged(displayName->text());

}


void Vector3DPropertyWidget::setConnections()
{
	connect(xSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double val) {
		x = val;
		emit valueChanged(QVector3D(x, y, z));
	});
	connect(ySpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double val) {
		y = val;
		emit valueChanged(QVector3D(x, y, z));
	});
	connect(zSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double val) {
		z = val;
		emit valueChanged(QVector3D(x, y, z));
	});

	connect(this, &Vector3DPropertyWidget::valueChanged, [=](QVector3D val) {
		setPropValues(val);
	});

}

void Vector3DPropertyWidget::setPropValues(QVector3D values) {
	x = values.x();
	y = values.y();
	z = values.z();
	value = values;

	prop->value.setX(x);
	prop->value.setY(y);
	prop->value.setZ(z);
}


/////////////////////////////




Vector4DPropertyWidget::Vector4DPropertyWidget() : BasePropertyWidget()
{
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	auto mainLayout = layout;
	
	auto wid = new Widget4D;
	xSpinBox = wid->xSpinBox;
	ySpinBox = wid->ySpinBox;
	zSpinBox = wid->zSpinBox;
	wSpinBox = wid->wSpinBox;
	setConnections();
	mainLayout->addWidget(wid);
	setLayout(mainLayout);

}


Vector4DPropertyWidget::~Vector4DPropertyWidget()
{
}

void Vector4DPropertyWidget::setProp(Vec4Property *prop)
{
	this->prop = prop;
	displayName->setText(prop->displayName);
	xSpinBox->setValue(prop->value.x());
	ySpinBox->setValue(prop->value.y());
	zSpinBox->setValue(prop->value.z());
	modelProperty = prop;
	emit nameChanged(displayName->text());
}

void Vector4DPropertyWidget::setConnections()
{
	connect(xSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double val) {
		x = val;
		emit valueChanged(QVector4D(x, y, z, w));
	});
	connect(ySpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double val) {
		y = val;
		emit valueChanged(QVector4D(x, y, z, w));
	});
	connect(zSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double val) {
		z = val;
		emit valueChanged(QVector4D(x, y, z, w));
	});
	connect(wSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double val) {
		w = val;
		emit valueChanged(QVector4D(x, y, z, w));
	});

	connect(this, &Vector4DPropertyWidget::valueChanged, [=](QVector4D val) {
		setPropValues(val);
	});
}


void Vector4DPropertyWidget::setPropValues(QVector4D values) {
	x = values.x();
	y = values.y();
	z = values.z();
	w = values.w();
	value = values;

	prop->value.setX(x);
	prop->value.setY(y);
	prop->value.setZ(z);
	prop->value.setW(w);
}





