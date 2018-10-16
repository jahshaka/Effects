#pragma once
#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>


class PropertyWidgetBase : public QWidget
{
public:
	PropertyWidgetBase();
	~PropertyWidgetBase();

	QLayout *layout;
};

class Widget2D : public PropertyWidgetBase
{
	Q_OBJECT
public:
	Widget2D();
	~Widget2D();

	void setXSpinBoxConnection(std::function<void(double val)> func = nullptr);
	void setYSpinBoxConnection(std::function<void(double val)> func = nullptr);

	QDoubleSpinBox* xSpinBox;
	QDoubleSpinBox* ySpinBox;
	double x;
	double y;
signals:
	void valueChanged(QVector2D val);

};


class Widget3D : public PropertyWidgetBase
{
	Q_OBJECT
public:
	Widget3D();
	~Widget3D();

	void setXSpinBoxConnection(std::function<void(double val)> func = nullptr);
	void setYSpinBoxConnection(std::function<void(double val)> func = nullptr);
	void setZSpinBoxConnection(std::function<void(double val)> func = nullptr);

	QDoubleSpinBox* xSpinBox;
	QDoubleSpinBox* ySpinBox;
	QDoubleSpinBox* zSpinBox;
	double x;
	double y;
	double z;
signals:
	void valueChanged(QVector3D val);

};


class Widget4D : public PropertyWidgetBase
{
	Q_OBJECT
public:
	Widget4D();
	~Widget4D();

	void setXSpinBoxConnection(std::function<void(double val)> func = nullptr);
	void setYSpinBoxConnection(std::function<void(double val)> func = nullptr);
	void setZSpinBoxConnection(std::function<void(double val)> func = nullptr);
	void setWSpinBoxConnection(std::function<void(double val)> func = nullptr);

	QDoubleSpinBox* xSpinBox;
	QDoubleSpinBox* ySpinBox;
	QDoubleSpinBox* zSpinBox;
	QDoubleSpinBox* wSpinBox;
	double x;
	double y;
	double z;
	double w;
signals:
	void valueChanged(QVector4D val);

};


enum class LabelState {
	Visible,
	Hidden
};

class WidgetInt : public PropertyWidgetBase
{
	Q_OBJECT
public:
	WidgetInt();
	WidgetInt(LabelState statue);
	~WidgetInt();

	void setIntSpinBoxConnection(std::function<void(int val)> func = nullptr);

	QSpinBox* spinBox;
	QSpinBox* maxSpinBox;
	QSpinBox* minSpinBox;
	QSpinBox* stepSpinBox;
	int value;
	int min = 0;
	int max = 0;
	int step = 0;
	LabelState state = LabelState::Visible;
signals:
	void valueChanged(int val);

};


class WidgetFloat : public PropertyWidgetBase
{
	Q_OBJECT
public:
	WidgetFloat();
	WidgetFloat(LabelState statue);
	~WidgetFloat();

	void setFloatSpinBoxConnection(std::function<void(double val)> func = nullptr);

	QDoubleSpinBox* floatSpinBox;
	QDoubleSpinBox* maxSpinBox;
	QDoubleSpinBox* minSpinBox;
	QDoubleSpinBox* stepSpinBox;
	double value;
	double min;
	double max;
	double step;

	LabelState state = LabelState::Visible;
signals:
	void valueChanged(double val);

};

class WidgetTexture : public PropertyWidgetBase
{
	Q_OBJECT
public:
	WidgetTexture();
	~WidgetTexture();

	QPushButton *texture;
	QString fileName;
};



