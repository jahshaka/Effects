#pragma once

#include <QWidget>
#include <QGridLayout>
#include <QSpinBox>
#include <QLineEdit>
#include <QPainter>
#include "properties.h"
#include "basepropertywidget.h"

class VectorPropertyWidget : public BasePropertyWidget
{
public:
	VectorPropertyWidget();
	~VectorPropertyWidget();
};

class Vector2DPropertyWidget : public BasePropertyWidget
{
	Q_OBJECT
public:
	Vector2DPropertyWidget();
	~Vector2DPropertyWidget();
	void setProp(Vec2Property *prop);
	QWidget* getValueWidget();
	QWidget* getWidget();


private:
	QDoubleSpinBox* xSpinBox;
	QDoubleSpinBox* ySpinBox;
	Vec2Property *prop;
	//QLineEdit *displayName;
	double x;
	double y;
	QVector2D value;
	QWidget *widget;

protected:

public slots:
	void setPropValues(QVector2D values);
signals:
	void valueChanged(QVector2D val);
	void nameChanged(QString name);


};

class Vector3DPropertyWidget : public BasePropertyWidget
{
	Q_OBJECT
public:
	Vector3DPropertyWidget();
	~Vector3DPropertyWidget();
	void setProp(Vec3Property *prop);
	QWidget* getValueWidget();
	QWidget* getWidget();


private:
	QDoubleSpinBox* xSpinBox;
	QDoubleSpinBox* ySpinBox;
	QDoubleSpinBox* zSpinBox;
	Vec3Property *prop;
	//QLineEdit *displayName;
	double x;
	double y;
	double z;
	QVector3D value;
	QWidget *widget;

protected:

public slots:
	void setPropValues(QVector3D values);
signals:
	void valueChanged(QVector3D val);
	void nameChanged(QString name);
};



class Vector4DPropertyWidget : public BasePropertyWidget
{
	Q_OBJECT
public:
	Vector4DPropertyWidget();
	~Vector4DPropertyWidget();
	void setProp(Vec4Property *prop);
	QWidget* getValueWidget();
	QWidget* getWidget();


private:
	QDoubleSpinBox* xSpinBox;
	QDoubleSpinBox* ySpinBox;
	QDoubleSpinBox* zSpinBox;
	QDoubleSpinBox* wSpinBox;
	Vec4Property *prop;
	//QLineEdit *displayName;
	double x;
	double y;
	double z;
	double w;
	QVector4D value;
	QWidget *widget;

protected:

public slots:
	void setPropValues(QVector4D values);
signals:
	void valueChanged(QVector4D val);	void nameChanged(QString name);

};

