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


private:
	QSpinBox* xSpinBox;
	QSpinBox* ySpinBox;
	Vec2Property *prop;
	QLineEdit *displayName;
	int x;
	int y;
	QVector2D value;

protected:
	void paintEvent(QPaintEvent *event) override;

public slots:
	void setPropValues(QVector2D values);
signals:
	void valueChanged(QVector2D val);


};

