#include <QWidget>
#include <QGridLayout>
#include <QSpinBox>
#include <QLineEdit>
#include <QPainter>
#include "properties.h"


class IntPropertyWidget : public QWidget
{
	Q_OBJECT
public:
	IntPropertyWidget();
	~IntPropertyWidget();
	void setProp(IntProperty *prop);
	int getValue();
	QWidget* getValueWidget();
	QWidget* getWidget();

private:
	QSpinBox* intSpinBox;
	QSpinBox* maxSpinBox;
	QSpinBox* minSpinBox;
	QSpinBox* stepSpinBox;
	IntProperty *prop;
	QLineEdit *displayName;
	int x = 0;
	int min =0;
	int max =0;
	int step =0;
	QWidget *widget;

protected:
	void paintEvent(QPaintEvent *event) override;

public slots:
	void setPropValue(int value);
signals:
	void valueChanged(int val);
	void nameChanged(QString name);

};