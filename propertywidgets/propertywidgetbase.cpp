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
	maxSpinBox = new QSpinBox(this);
	minSpinBox = new QSpinBox(this);
	stepSpinBox = new QSpinBox(this);

	auto label = new QLabel("Value");
	auto label1 = new QLabel("Min");
	auto label2 = new QLabel("Max");
	auto label3 = new QLabel("Step");


	auto vbox = new QHBoxLayout;
	auto minbox = new QHBoxLayout;
	auto maxbox = new QHBoxLayout;
	auto stepbox = new QHBoxLayout;

	vbox->addWidget(label);
	vbox->addWidget(spinBox);
	minbox->addWidget(label2);
	minbox->addWidget(maxSpinBox);
	maxbox->addWidget(label1);
	maxbox->addWidget(minSpinBox);
	stepbox->addWidget(label3);
	stepbox->addWidget(stepSpinBox);

	auto wid = new QWidget;
	auto widLayout = new QVBoxLayout;
	wid->setLayout(widLayout);

	widLayout->addLayout(vbox);
	widLayout->addLayout(minbox);
	widLayout->addLayout(maxbox);
	widLayout->addLayout(stepbox);

	layout->addWidget(wid);
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

	connect(minSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int val) {
		spinBox->setMinimum(val);
	});
	connect(maxSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int val) {
		spinBox->setMaximum(val);

	});
	connect(stepSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int val) {
		spinBox->setSingleStep(val);
	});

}



WidgetFloat::WidgetFloat() : PropertyWidgetBase()
{
	floatSpinBox = new QDoubleSpinBox;
	maxSpinBox = new QDoubleSpinBox(this);
	minSpinBox = new QDoubleSpinBox(this);
	stepSpinBox = new QDoubleSpinBox(this);

	auto label = new QLabel("Value");
	auto label1 = new QLabel("Min");
	auto label2 = new QLabel("Max");
	auto label3 = new QLabel("Step");


	auto vbox = new QHBoxLayout;
	auto minbox = new QHBoxLayout;
	auto maxbox = new QHBoxLayout;
	auto stepbox = new QHBoxLayout;

	vbox->addWidget(label);
	vbox->addWidget(floatSpinBox);
	minbox->addWidget(label2);
	minbox->addWidget(maxSpinBox);
	maxbox->addWidget(label1);
	maxbox->addWidget(minSpinBox);
	stepbox->addWidget(label3);
	stepbox->addWidget(stepSpinBox);

	auto wid = new QWidget;
	auto widLayout = new QVBoxLayout;
	wid->setLayout(widLayout);

	widLayout->addLayout(vbox);
	widLayout->addLayout(minbox);
	widLayout->addLayout(maxbox);
	widLayout->addLayout(stepbox);

	layout->addWidget(wid);
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
	connect(floatSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double val) {
		value = val;
		func(val);
		emit valueChanged(value);
	});
}
