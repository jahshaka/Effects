#include <QWidget>
#include <QGridLayout>
#include <QSpinBox>
#include <QLineEdit>
#include <QPainter>
#include "properties.h"
#include "basepropertywidget.h"


class FloatPropertyWidget : public BasePropertyWidget
{
	Q_OBJECT
public:
	FloatPropertyWidget();
	~FloatPropertyWidget();
	void setProp(FloatProperty *prop);
	float getValue();
	QWidget* getValueWidget();
	QWidget* getWidget();

private:
	QDoubleSpinBox* intSpinBox;
	QDoubleSpinBox* maxSpinBox;
	QDoubleSpinBox* minSpinBox;
	QDoubleSpinBox* stepSpinBox;
	FloatProperty *prop;
	float x = 0;
	float min =0;
	float max =0;
	float step =0;
	QWidget *widget;

protected:

public slots:
	void setPropValue(double value);
signals:
	void valueChanged(double val);
	void nameChanged(QString name);

};