#pragma once
#include <QWidget>
#include <QFileDialog>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QPainter>
#include <QPushButton>
#include "properties.h"


class TexturePropertyWidget : public QWidget
{
	Q_OBJECT
public:
	TexturePropertyWidget();
	~TexturePropertyWidget();
	void setProp(TextureProperty *prop);
	QString getValue();
	QWidget* getValueWidget();
	QWidget* getWidget();


private:
	TextureProperty *prop;
	QLineEdit *displayName;
	QWidget *widget;

	QString value;
	QPushButton *texture;

protected:
	void paintEvent(QPaintEvent *event) override;

public slots:
	void setPropValue(QString value);
signals:
	void valueChanged(QString val);
	void nameChanged(QString name);
};

