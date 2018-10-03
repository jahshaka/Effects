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
	auto label = new QLabel("Display Name", this);

	textLayout->addWidget(label);
	textLayout->addStretch();
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
	emit nameChanged(displayName->text());

}

QWidget * Vector2DPropertyWidget::getValueWidget()
{

	widget = new QWidget(this);
	auto spinLayout = new QHBoxLayout();
	auto label = new QLabel("Values", this);

	spinLayout->setContentsMargins(0, 0, 0, 0);


	widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	widget->setLayout(spinLayout);

	xSpinBox = new QDoubleSpinBox(this);
	ySpinBox = new QDoubleSpinBox(this);

	spinLayout->addWidget(label);
	spinLayout->addWidget(xSpinBox);
	spinLayout->addWidget(ySpinBox);

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


	return widget;
}

QWidget * Vector2DPropertyWidget::getWidget()
{
	if (widget) return widget;
	return nullptr;
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


/////////////////////////////




Vector3DPropertyWidget::Vector3DPropertyWidget() : QWidget()
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
	emit nameChanged(displayName->text());

}

QWidget * Vector3DPropertyWidget::getValueWidget()
{

	widget = new QWidget(this);
	auto spinLayout = new QHBoxLayout();
	auto label = new QLabel("Values", this);

	spinLayout->setContentsMargins(0, 0, 0, 0);


	widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	widget->setLayout(spinLayout);

	xSpinBox = new QDoubleSpinBox(this);
	ySpinBox = new QDoubleSpinBox(this);
	zSpinBox = new QDoubleSpinBox(this);

	spinLayout->addWidget(label);
	spinLayout->addWidget(xSpinBox);
	spinLayout->addWidget(ySpinBox);
	spinLayout->addWidget(zSpinBox);

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


	return widget;
}

QWidget * Vector3DPropertyWidget::getWidget()
{
	if (widget) return widget;
	return nullptr;
}

void Vector3DPropertyWidget::setPropValues(QVector3D values) {
	x = values.x();
	y = values.y();
	z = values.z();
	value = values;
}

void Vector3DPropertyWidget::paintEvent(QPaintEvent * event)
{
	QWidget::paintEvent(event);
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(QPen(QColor(200, 200, 200, 70), 2));
	painter.drawRect(0, 0, width(), height());
}


/////////////////////////////




Vector4DPropertyWidget::Vector4DPropertyWidget() : QWidget()
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
	emit nameChanged(displayName->text());
}

QWidget * Vector4DPropertyWidget::getValueWidget()
{

	widget = new QWidget(this);
	auto spinLayout = new QHBoxLayout();
	auto label = new QLabel("Values", this);

	spinLayout->setContentsMargins(0, 0, 0, 0);


	widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	widget->setLayout(spinLayout);

	xSpinBox = new QDoubleSpinBox(this);
	ySpinBox = new QDoubleSpinBox(this);
	zSpinBox = new QDoubleSpinBox(this);
	wSpinBox = new QDoubleSpinBox(this);

	spinLayout->addWidget(label);
	spinLayout->addWidget(xSpinBox);
	spinLayout->addWidget(ySpinBox);
	spinLayout->addWidget(zSpinBox);
	spinLayout->addWidget(wSpinBox);

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


	return widget;
}

QWidget * Vector4DPropertyWidget::getWidget()
{
	if (widget) return widget;
	return nullptr;
}

void Vector4DPropertyWidget::setPropValues(QVector4D values) {
	x = values.x();
	y = values.y();
	z = values.z();
	w = values.w();
	value = values;
}

void Vector4DPropertyWidget::paintEvent(QPaintEvent * event)
{
	QWidget::paintEvent(event);
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(QPen(QColor(200, 200, 200, 70), 2));
	painter.drawRect(0, 0, width(), height());
}



