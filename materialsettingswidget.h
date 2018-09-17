#ifndef MATERIALSETTINGSWIDGET_H
#define MATERIALSETTINGSWIDGET_H

#include <QWidget>

namespace Ui {
class MaterialSettingsWidget;
}

class MaterialSettingsWidget : public QWidget
{
	Q_OBJECT

public:
	explicit MaterialSettingsWidget(QWidget *parent = 0);
	~MaterialSettingsWidget();

private:
	Ui::MaterialSettingsWidget* ui;
};

#endif
