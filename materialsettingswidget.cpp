#include "materialsettingswidget.h"
#include "ui_materialsettingswidget.h"


MaterialSettingsWidget::MaterialSettingsWidget(QWidget *parent):
	QWidget(parent),
	ui(new Ui::MaterialSettingsWidget)
{
	ui->setupUi(this);
}

MaterialSettingsWidget::~MaterialSettingsWidget()
{
	delete ui;
}