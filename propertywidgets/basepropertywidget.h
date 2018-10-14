#pragma once
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include "properties.h"

class BasePropertyWidget : public QWidget
{
	Q_OBJECT
public:
	BasePropertyWidget(QWidget *parent = nullptr);
	~BasePropertyWidget();

	QLineEdit *displayName;
	QWidget *displayWidget;
	QWidget *contentWidget = 0;
	QVBoxLayout *layout;
	Property *modelProperty;

	bool pressed = false;
	bool minimized = false;
	int index = 0;

protected:
	void paintEvent(QPaintEvent *event) override;

private:
	QPushButton *button;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;

signals:
	void buttonPressed();
	void currentWidget(BasePropertyWidget *widget);
	void shouldSetVisible(bool val);
	//void requestMimeData(Property *prop);
};

