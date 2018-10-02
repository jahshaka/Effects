#pragma once

#include <QWidget>
#include <QGridLayout>
#include <QSpinBox>
#include <QLineEdit>
#include <QPainter>
#include "properties.h"

class VectorPropertyWidget : public QWidget
{
public:
	VectorPropertyWidget();
	~VectorPropertyWidget();
};

class Vector2DPropertyWidget : public QWidget
{
	Q_OBJECT
public:
	Vector2DPropertyWidget();
	~Vector2DPropertyWidget();
	void setProp(Vec2Property *prop);
	QWidget* getValueWidget();
	QWidget* getWidget();


private:
	QSpinBox* xSpinBox;
	QSpinBox* ySpinBox;
	Vec2Property *prop;
	QLineEdit *displayName;
	int x;
	int y;
	QVector2D value;
	QWidget *widget;

protected:
	void paintEvent(QPaintEvent *event) override;

public slots:
	void setPropValues(QVector2D values);
signals:
	void valueChanged(QVector2D val);


};

class Vector3DPropertyWidget : public QWidget
{
	Q_OBJECT
public:
	Vector3DPropertyWidget();
	~Vector3DPropertyWidget();
	void setProp(Vec3Property *prop);
	QWidget* getValueWidget();
	QWidget* getWidget();


private:
	QSpinBox* xSpinBox;
	QSpinBox* ySpinBox;
	QSpinBox* zSpinBox;
	Vec3Property *prop;
	QLineEdit *displayName;
	int x;
	int y;
	int z;
	QVector3D value;
	QWidget *widget;

protected:
	void paintEvent(QPaintEvent *event) override;

public slots:
	void setPropValues(QVector3D values);
signals:
	void valueChanged(QVector3D val);
};



class Vector4DPropertyWidget : public QWidget
{
	Q_OBJECT
public:
	Vector4DPropertyWidget();
	~Vector4DPropertyWidget();
	void setProp(Vec4Property *prop);
	QWidget* getValueWidget();
	QWidget* getWidget();


private:
	QSpinBox* xSpinBox;
	QSpinBox* ySpinBox;
	QSpinBox* zSpinBox;
	QSpinBox* wSpinBox;
	Vec4Property *prop;
	QLineEdit *displayName;
	int x;
	int y;
	int z;
	int w;
	QVector4D value;
	QWidget *widget;

protected:
	void paintEvent(QPaintEvent *event) override;

public slots:
	void setPropValues(QVector4D values);
signals:
	void valueChanged(QVector4D val);
};

