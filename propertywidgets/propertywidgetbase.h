#pragma once
#include <QWidget>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>


class PropertyWidgetBase : public QWidget
{
public:
	PropertyWidgetBase();
	~PropertyWidgetBase();

	QHBoxLayout *layout;
};

class Widget2D : PropertyWidgetBase
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


class Widget3D : PropertyWidgetBase
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


class Widget4D : PropertyWidgetBase
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

class WidgetInt : PropertyWidgetBase
{
	Q_OBJECT
public:
	WidgetInt();
	WidgetInt(LabelState statue);
	~WidgetInt();

	void setIntSpinBoxConnection(std::function<void(int val)> func = nullptr);

	QSpinBox* spinBox;
	int value;
	LabelState state = LabelState::Visible;
signals:
	void valueChanged(int val);

};


class WidgetFloat : PropertyWidgetBase
{
	Q_OBJECT
public:
	WidgetFloat();
	WidgetFloat(LabelState statue);
	~WidgetFloat();

	void setFloatSpinBoxConnection(std::function<void(double val)> func = nullptr);

	QDoubleSpinBox* spinBox;
	double value;
	LabelState state = LabelState::Visible;
signals:
	void valueChanged(double val);

};

