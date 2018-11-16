#pragma once
#include <QDialog>
#include <QLineEdit>
#include <QGroupBox>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>
#include "../listwidget.h"
#include <QWidget>
#include <QScrollArea>
struct nodeGraphPreset {
	QString name = "";
	QString title = "";
	QString iconPath = "";
};

class OptionSelection : public QPushButton
{
	Q_OBJECT
public:
	OptionSelection(nodeGraphPreset node);

	nodeGraphPreset info;
	QPixmap checkedIconIcon;
protected:
    void paintEvent(QPaintEvent *event) override;

signals:
	void buttonSelected(OptionSelection* button);
	void OptionSelected(nodeGraphPreset info);
};

class CreateNewDialog : public QDialog
{
public:
	CreateNewDialog(QList<nodeGraphPreset> list);
	~CreateNewDialog();

	void configureStylesheet();
private:
	QPushButton * cancel;
	QPushButton * confirm;
	QLineEdit * nameEdit;
	QWidget* options;
	QWidget* presets;
	QWidget *holder;
	QTabWidget *tabbedWidget;
	QScrollArea *optionsScroll;
	QScrollArea *presetsScroll;
    QWidget *optionsWidget;
    QWidget *presetsWidget;
	nodeGraphPreset currentInfoSelected;
	QLabel *infoLabel;
    int num_of_widgets_per_row = 3;



};



