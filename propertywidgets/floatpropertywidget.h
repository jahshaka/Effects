#include <QWidget>
#include <QGridLayout>
#include <QSpinBox>
#include <QLineEdit>
#include <QPainter>
#include "properties.h"


class FloatPropertyWidget : public QWidget
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
	QLineEdit *displayName;
	float x = 0;
	float min =0;
	float max =0;
	float step =0;
	QWidget *widget;

protected:
	void paintEvent(QPaintEvent *event) override;

public slots:
	void setPropValue(double value);
signals:
	void valueChanged(double val);





};