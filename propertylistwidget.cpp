#include "propertylistwidget.h"
#include "ui_propertylistwidget.h"
#include <QMenu>
#include <QDebug>
#include <QScrollArea>
#include <QToolButton>
#include "propertywidgets/floatpropertywidget.h"
#include "propertywidgets/vectorpropertywidget.h"
#include "propertywidgets/intpropertywidget.h"
#include "propertywidgets/texturepropertywidget.h"
#include "properties.h"
#include "graph/nodegraph.h"
//#include "nodemodel.h"

PropertyListWidget::PropertyListWidget(QWidget *parent) :
    QWidget(parent)
{
    auto menu = new QMenu(this);
    auto action = menu->addAction("Add Float Property");
    auto actionInt = menu->addAction("Add Int Property");
	auto action2 = menu->addAction("Add Vector2 Property");
	auto action3 = menu->addAction("Add Vector3 Property");
	auto action4 = menu->addAction("Add Vector4 Property");
    auto action5 = menu->addAction("Add Texture Property");




    layout = new QVBoxLayout();
    layout->addStretch();
	layout->setSpacing(15);

	auto mainLayout = new QVBoxLayout;
	auto addProp = new QToolButton;
	auto scrollArea = new QScrollArea;
	auto contentWidget = new QWidget;

	addProp->setText(tr("Add Property"));
	addProp->setMenu(menu);
	addProp->setPopupMode(QToolButton::InstantPopup);
	addProp->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	contentWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
	contentWidget->setMinimumWidth(200);

	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->addSpacing(15);
	mainLayout->addWidget(addProp);
	mainLayout->addSpacing(5);
	mainLayout->addWidget(scrollArea);

	contentWidget->setLayout(layout);
	scrollArea->setWidget(contentWidget);
	scrollArea->setWidgetResizable(true);
	scrollArea->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
	scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	setLayout(mainLayout);

	connect(action, &QAction::triggered, this, &PropertyListWidget::addNewFloatProperty);
	connect(actionInt, &QAction::triggered, this, &PropertyListWidget::addNewIntProperty);
	connect(action2, &QAction::triggered, this, &PropertyListWidget::addNewVec2Property);
	connect(action3, &QAction::triggered, this, &PropertyListWidget::addNewVec3Property);
	connect(action4, &QAction::triggered, this, &PropertyListWidget::addNewVec4Property);
	connect(action5, &QAction::triggered, this, &PropertyListWidget::addNewTextureProperty);

	scrollArea->setStyleSheet(""
		"QScrollBar:vertical {border : 0px solid black;	background: rgba(132, 132, 132, 0);width: 24px; padding: 4px;}"
		"QScrollBar::handle{ background: rgba(72, 72, 72, 1);	border-radius: 8px; width: 14px; }"
		"QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {	background: rgba(200, 200, 200, 0);}"
		"QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical {	background: rgba(0, 0, 0, 0);border: 0px solid white;}"
		"QScrollBar::sub-line, QScrollBar::add-line {	background: rgba(10, 0, 0, .0);}"
	);
}

PropertyListWidget::~PropertyListWidget()
{
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
	added++;
	auto propWidget = new FloatPropertyWidget();
	propWidget->index = added;
	this->layout->insertWidget(this->layout->count() - 1, propWidget);
	propWidget->setProp(floatProp);
	connect(propWidget, &BasePropertyWidget::currentWidget, [=](BasePropertyWidget *wid) {
		currentWidget = wid;
	});

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
	added++;
	auto propWidget = new Vector2DPropertyWidget();
	propWidget->index = added;
	this->layout->insertWidget(this->layout->count() - 1, propWidget);
	propWidget->setProp(vec2Prop);
	connect(propWidget, &BasePropertyWidget::currentWidget, [=](BasePropertyWidget *wid) {
		currentWidget = wid;
	});
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
	added++;
	auto propWidget = new Vector3DPropertyWidget();
	propWidget->index = added;
	this->layout->insertWidget(this->layout->count() - 1, propWidget);
	propWidget->setProp(vec3Prop);
	connect(propWidget, &BasePropertyWidget::currentWidget, [=](BasePropertyWidget *wid) {
		currentWidget = wid;
	});
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
	added++;
	auto propWidget = new Vector4DPropertyWidget();
	propWidget->index = added;
	this->layout->insertWidget(this->layout->count() - 1, propWidget);
	propWidget->setProp(vec4Prop);
	connect(propWidget, &BasePropertyWidget::currentWidget, [=](BasePropertyWidget *wid) {
		currentWidget = wid;
	});
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
	added++;
	auto propWidget = new IntPropertyWidget();
	propWidget->index = added;
	this->layout->insertWidget(this->layout->count() - 1, propWidget);
	propWidget->setProp(intProp);
	connect(propWidget, &BasePropertyWidget::currentWidget, [=](BasePropertyWidget *wid) {
		currentWidget = wid;
	});
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
	added++;
	auto propWidget = new TexturePropertyWidget;
	propWidget->index = added;
	this->layout->insertWidget(this->layout->count() - 1, propWidget);
	propWidget->setProp(texProp);
	connect(propWidget, &BasePropertyWidget::currentWidget, [=](BasePropertyWidget *wid) {
		currentWidget = wid;
	});
}
