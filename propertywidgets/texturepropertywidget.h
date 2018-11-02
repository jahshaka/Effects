#pragma once
#include <QWidget>
#include <QFileDialog>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QPainter>
#include <QPushButton>
#include "properties.h"
#include "basepropertywidget.h"
#include "propertywidgetbase.h"

class GraphTexture;
class TexturePropertyWidget : public BasePropertyWidget
{
	Q_OBJECT
public:
	TexturePropertyWidget();
	~TexturePropertyWidget();
	void setProp(TextureProperty *prop);
	QString getValue();


private:
	TextureProperty *prop;
	GraphTexture* graphTexture;
	WidgetTexture *wid;
	QString value;
	QPushButton *texture;
	void setConnections();

public slots:
	void setPropValue(QString value);
signals:
	void valueChanged(QString val);
	void nameChanged(QString name);
};

