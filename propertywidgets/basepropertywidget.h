#pragma once
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>

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
protected:
	void paintEvent(QPaintEvent *event) override;

private:
	QPushButton *button;

signals:
	void buttonPressed();
};

