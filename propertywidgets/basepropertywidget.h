#pragma once
#include <QWidget>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include "properties.h"

class HeaderObject : public QWidget
{
	Q_OBJECT
public:
	HeaderObject();

protected:
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event);
	void enterEvent(QEvent *event);
};

class BasePropertyWidget : public QWidget
{
	Q_OBJECT
public:
	BasePropertyWidget(QWidget *parent = nullptr);
	~BasePropertyWidget();

	QLineEdit *displayName;
	HeaderObject *displayWidget;
	QWidget *contentWidget = 0;
	QVBoxLayout *layout;
	Property *modelProperty;
	QPropertyAnimation *anim;

	bool pressed = false;
	bool minimized = false;
	int index = 0;

	void setWidget(QWidget *widget);

protected:
	void paintEvent(QPaintEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;


private:
	QPushButton *button;
	QSize currentSize;
	void animateMinimize();
	void animateMaximize();  

signals:
	void buttonPressed();
	void buttonClose();
	void buttonResize(bool resize);
	void currentWidget(BasePropertyWidget *widget);
	void shouldSetVisible(bool val);
	void TitleChanged(QString title);
	//void requestMimeData(Property *prop);
};

