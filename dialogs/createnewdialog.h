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

struct dialogType {

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
	Q_OBJECT
public:
	CreateNewDialog(QList<nodeGraphPreset> list);
	~CreateNewDialog();

	void configureStylesheet();
	QString getName() { return name; }
	QString getTemplateName() { return templateName; }
	int getType() { return type; }


private:
	QString name;
	int type; // presets = 1, assets =2 
	QString templateName;

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

signals:
	void confirmClicked(int option);


};



