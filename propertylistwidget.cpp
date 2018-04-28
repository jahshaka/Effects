#include "propertylistwidget.h"
#include "ui_propertylistwidget.h"

PropertyListWidget::PropertyListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PropertyListWidget)
{
    ui->setupUi(this);
}

PropertyListWidget::~PropertyListWidget()
{
    delete ui;
}
