#include "propertywidgetbase.h"
#include <QLabel>


PropertyWidgetBase::PropertyWidgetBase() : QWidget()
{
	layout = new QHBoxLayout;
	setLayout(layout);
}


PropertyWidgetBase::~PropertyWidgetBase()
{
}


Widget2D::Widget2D() : PropertyWidgetBase()
{
	xSpinBox = new QDoubleSpinBox;
	ySpinBox = new QDoubleSpinBox;
	auto xLabel = new QLabel("X");
	auto yLabel = new QLabel("Y");

	layout->addWidget(xLabel);
	layout->addWidget(xSpinBox);
	layout->addWidget(yLabel);
	layout->addWidget(ySpinBox);
}

Widget2D::~Widget2D()
{
}

void Widget2D::setXSpinBoxConnection(std::function<void(double val)> func)
{
	connect(xSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double val) {
		x = val;
		func(val);
		emit valueChanged(QVector2D(x, y));
	});
}

void Widget2D::setYSpinBoxConnection(std::function<void(double val)> func)
{
	connect(ySpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double val) {
		y = val;
		func(val);
		emit valueChanged(QVector2D(x, y));
	});
}




Widget3D::Widget3D() : PropertyWidgetBase()
{
	xSpinBox = new QDoubleSpinBox;
	ySpinBox = new QDoubleSpinBox;
	zSpinBox = new QDoubleSpinBox;
	auto xLabel = new QLabel("X");
	auto yLabel = new QLabel("Y");
	auto zLabel = new QLabel("Z");

	layout->addWidget(xLabel);
	layout->addWidget(xSpinBox);
	layout->addWidget(yLabel);
	layout->addWidget(ySpinBox);
	layout->addWidget(zLabel);
	layout->addWidget(zSpinBox);
}

Widget3D::~Widget3D()
{
}

void Widget3D::setXSpinBoxConnection(std::function<void(double val)> func)
{
	connect(xSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double val) {
		x = val;
		func(val);
		emit valueChanged(QVector3D(x, y, z));
	});
}

void Widget3D::setYSpinBoxConnection(std::function<void(double val)> func)
{
	connect(ySpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double val) {
		y = val;
		func(val);
		emit valueChanged(QVector3D(x, y, z));
	});
}

void Widget3D::setZSpinBoxConnection(std::function<void(double val)> func)
{
	connect(zSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double val) {
		z = val;
		func(val);
		emit valueChanged(QVector3D(x, y, z));
	});
}




Widget4D::Widget4D() : PropertyWidgetBase()
{
	xSpinBox = new QDoubleSpinBox;
	ySpinBox = new QDoubleSpinBox;
	zSpinBox = new QDoubleSpinBox;
	wSpinBox = new QDoubleSpinBox;
	auto xLabel = new QLabel("X");
	auto yLabel = new QLabel("Y");
	auto zLabel = new QLabel("Z");
	auto wLabel = new QLabel("W");

	layout->addWidget(xLabel);
	layout->addWidget(xSpinBox);
	layout->addWidget(yLabel);
	layout->addWidget(ySpinBox);
	layout->addWidget(zLabel);
	layout->addWidget(zSpinBox);
	layout->addWidget(wLabel);
	layout->addWidget(wSpinBox);
}

Widget4D::~Widget4D()
{
}

void Widget4D::setXSpinBoxConnection(std::function<void(double val)> func)
{
	connect(xSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double val) {
		x = val;
		func(val);
		emit valueChanged(QVector4D(x, y, z, w));
	});
}

void Widget4D::setYSpinBoxConnection(std::function<void(double val)> func)
{
	connect(ySpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double val) {
		y = val;
		func(val);
		emit valueChanged(QVector4D(x, y, z, w));
	});
}

void Widget4D::setZSpinBoxConnection(std::function<void(double val)> func)
{
	connect(zSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double val) {
		z = val;
		func(val);
		emit valueChanged(QVector4D(x, y, z, w));
	});
}


void Widget4D::setWSpinBoxConnection(std::function<void(double val)> func)
{
	connect(zSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double val) {
		w = val;
		func(val);
		emit valueChanged(QVector4D(x, y, z, w));
	});
}


WidgetInt::WidgetInt() : PropertyWidgetBase()
{
	spinBox = new QSpinBox;
	auto label = new QLabel("Value");

	if (state == LabelState::Visible)
		layout->addWidget(label);
	layout->addWidget(spinBox);
}


WidgetInt::WidgetInt(LabelState state)
{
	this->state = state;
	WidgetInt::WidgetInt();
}

WidgetInt::~WidgetInt()
{
}

void WidgetInt::setIntSpinBoxConnection(std::function<void(int val)> func)
{
	connect(spinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int val) {
		value = val;
		func(val);
		emit valueChanged(value);
	});
}



WidgetFloat::WidgetFloat() : PropertyWidgetBase()
{
	spinBox = new QDoubleSpinBox;
	auto label = new QLabel("Value");

	if (state == LabelState::Visible)
		layout->addWidget(label);
	layout->addWidget(spinBox);
}


WidgetFloat::WidgetFloat(LabelState state)
{
	this->state = state;
	WidgetFloat::WidgetFloat();
}

WidgetFloat::~WidgetFloat()
{
}

void WidgetFloat::setFloatSpinBoxConnection(std::function<void(double val)> func)
{
	connect(spinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double val) {
		value = val;
		func(val);
		emit valueChanged(value);
	});
}
