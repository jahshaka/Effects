#include "propertylistwidget.h"
#include "ui_propertylistwidget.h"
#include <QMenu>
#include <QDebug>
#include "propertywidgets/floatpropertywidget.h"
#include "properties.h"
#include "nodegraph.h"
#include "nodemodel.h"

PropertyListWidget::PropertyListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PropertyListWidget)
{
    ui->setupUi(this);
    auto menu = new QMenu(this);
    auto action = menu->addAction("Add Float Property");
    connect(action, &QAction::triggered, this, &PropertyListWidget::addNewFloatProperty);

    menu->addAction("Add Int Property");
    menu->addAction("Add Vector2 Property");
    menu->addAction("Add Vector3 Property");
    menu->addAction("Add Vector4 Property");
    menu->addAction("Add Texture Property");
    ui->addPropertyButton->setMenu(menu);
    ui->addPropertyButton->setPopupMode(QToolButton::InstantPopup);

    delete ui->widgetList->layout();
    layout = new QVBoxLayout();
    layout->addStretch();
    ui->widgetList->setLayout(layout);
}

PropertyListWidget::~PropertyListWidget()
{
    delete ui;
}

void PropertyListWidget::addProperty(QWidget *widget)
{

}

void PropertyListWidget::setNodeGraph(NodeGraph *graph)
{
	//todo: clear widgets

    this->graph = graph;

    // build properties
	for (auto prop : graph->properties) {
		switch (prop->type) {
		case PropertyType::Float:
			addFloatProperty((FloatProperty*)prop);
		default:
			break;
		}
	}
}

void PropertyListWidget::addNewFloatProperty()
{
    auto prop = new FloatProperty();
    prop->displayName = "Float Property";
    // todo: not safe! find a better way to do this!!!!
    prop->name = QString("property%1").arg(graph->properties.count());
	this->addFloatProperty(prop);
	this->graph->addProperty(prop);
}

void PropertyListWidget::addFloatProperty(FloatProperty* floatProp)
{
	auto propWidget = new FloatPropertyWidget();
	this->layout->insertWidget(this->layout->count() - 1, propWidget);
	propWidget->setProperty(floatProp);
}
