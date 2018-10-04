#include "propertylistwidget.h"
#include "ui_propertylistwidget.h"
#include <QMenu>
#include <QDebug>
#include "propertywidgets/floatpropertywidget.h"
#include "propertywidgets/vectorpropertywidget.h"
#include "propertywidgets/intpropertywidget.h"
#include "propertywidgets/texturepropertywidget.h"
#include "properties.h"
#include "graph/nodegraph.h"
//#include "nodemodel.h"

PropertyListWidget::PropertyListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PropertyListWidget)
{
    ui->setupUi(this);
    auto menu = new QMenu(this);
    auto action = menu->addAction("Add Float Property");
    connect(action, &QAction::triggered, this, &PropertyListWidget::addNewFloatProperty);

    auto actionInt = menu->addAction("Add Int Property");
	auto action2 = menu->addAction("Add Vector2 Property");
	auto action3 = menu->addAction("Add Vector3 Property");
	auto action4 = menu->addAction("Add Vector4 Property");
    auto action5 = menu->addAction("Add Texture Property");
    ui->addPropertyButton->setMenu(menu);
    ui->addPropertyButton->setPopupMode(QToolButton::InstantPopup);

	connect(actionInt, &QAction::triggered, this, &PropertyListWidget::addNewIntProperty);
	connect(action2, &QAction::triggered, this, &PropertyListWidget::addNewVec2Property);
	connect(action3, &QAction::triggered, this, &PropertyListWidget::addNewVec3Property);
	connect(action4, &QAction::triggered, this, &PropertyListWidget::addNewVec4Property);
	connect(action5, &QAction::triggered, this, &PropertyListWidget::addNewTextureProperty);


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
	propWidget->setProp(floatProp);
}

void PropertyListWidget::addNewVec2Property()
{
	auto prop = new Vec2Property;
	prop->displayName = "Vector 2 property";
	prop->name = QString("property%1").arg(graph->properties.count());
	this->addVec2Property(prop);
	this->graph->addProperty(prop);
}

void PropertyListWidget::addVec2Property(Vec2Property * vec2Prop)
{
	auto propWidget = new Vector2DPropertyWidget();
	this->layout->insertWidget(this->layout->count() - 1, propWidget);
	propWidget->setProp(vec2Prop);
}
///////

void PropertyListWidget::addNewVec3Property()
{
	auto prop = new Vec3Property;
	prop->displayName = "Vector 3 property";
	prop->name = QString("property%1").arg(graph->properties.count());
	this->addVec3Property(prop);
	this->graph->addProperty(prop);
}

void PropertyListWidget::addVec3Property(Vec3Property * vec3Prop)
{
	auto propWidget = new Vector3DPropertyWidget();
	this->layout->insertWidget(this->layout->count() - 1, propWidget);
	propWidget->setProp(vec3Prop);
}

///////

void PropertyListWidget::addNewVec4Property()
{
	auto prop = new Vec4Property;
	prop->displayName = "Vector 4 property";
	prop->name = QString("property%1").arg(graph->properties.count());
	this->addVec4Property(prop);
	this->graph->addProperty(prop);
}

void PropertyListWidget::addVec4Property(Vec4Property * vec4Prop)
{
	auto propWidget = new Vector4DPropertyWidget();
	this->layout->insertWidget(this->layout->count() - 1, propWidget);
	propWidget->setProp(vec4Prop);
}

///////

void PropertyListWidget::addNewIntProperty()
{
	auto prop = new IntProperty;
	prop->displayName = "Int property";
	prop->name = QString("property%1").arg(graph->properties.count());
	this->addIntProperty(prop);
	this->graph->addProperty(prop);
}

void PropertyListWidget::addIntProperty(IntProperty * intProp)
{
	auto propWidget = new IntPropertyWidget();
	this->layout->insertWidget(this->layout->count() - 1, propWidget);
	propWidget->setProp(intProp);
}

////

void PropertyListWidget::addNewTextureProperty()
{
	auto prop = new TextureProperty;
	prop->displayName = "Texture property";
	prop->name = QString("property%1").arg(graph->properties.count());
	this->addTextureProperty(prop);
	this->graph->addProperty(prop);
}

void PropertyListWidget::addTextureProperty(TextureProperty * texProp)
{
	auto propWidget = new TexturePropertyWidget;
	this->layout->insertWidget(this->layout->count() - 1, propWidget);
	propWidget->setProp(texProp);
}
