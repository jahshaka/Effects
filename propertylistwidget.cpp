#include "propertylistwidget.h"
//#include "ui_propertylistwidget.h"
#include <QMenu>
#include <QDebug>
#include <QFont>
#include <qgraphicseffect.h>
#include <QScrollArea>
#include <QToolButton>
#include "propertywidgets/floatpropertywidget.h"
#include "propertywidgets/vectorpropertywidget.h"
#include "propertywidgets/intpropertywidget.h"
#include "propertywidgets/texturepropertywidget.h"
#include "properties.h"
#include "graph/nodegraph.h"
#include "mainwindow.h"
//#include "nodemodel.h"

PropertyListWidget::PropertyListWidget(QWidget *parent) :
    QWidget(parent)
{
    auto menu = new QMenu(this);
//	menu->setWindowFlag(Qt::NoDropShadowWindowHint );
	menu->setAttribute(Qt::WA_TranslucentBackground);
    auto action = menu->addAction		("Float ");
    auto actionInt = menu->addAction	("Int ");
	auto action2 = menu->addAction		("Vector 2 ");
	auto action3 = menu->addAction		("Vector 3 ");
	auto action4 = menu->addAction		("Vector 4 ");
    auto action5 = menu->addAction		("Texture ");

    layout = new QVBoxLayout();
    layout->addStretch();
    layout->setSpacing(15);

	auto mainLayout = new QVBoxLayout;
	auto addProp = new QToolButton;
	auto scrollArea = new QScrollArea;
	auto contentWidget = new QWidget;
	auto scrollLayout = new QHBoxLayout;

	QPushButton *pushButton = new QPushButton(QIcon(":/icons/add_object.svg"), "  Add Property");
	pushButton->setCursor(Qt::PointingHandCursor);
	pushButton->setMinimumWidth(110);

	menu->setFixedWidth(pushButton->width()/5.2);
	QObject::connect(pushButton, &QPushButton::released, [=]() {
		QPoint pos = this->mapToGlobal(pushButton->pos());
		pos += QPoint(0, pushButton->height());
		menu->exec(pos);
	});

	auto addLayout = new QHBoxLayout;
	int space = 165;
	addLayout->addStretch();
	addLayout->addWidget(pushButton);
	addLayout->addSpacing(8);

//	scrollLayout->addStretch();
//	scrollLayout->addWidget(scrollArea);
//	scrollLayout->addStretch();

	scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	contentWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	contentWidget->setMinimumWidth(200);

	mainLayout->setContentsMargins(3, 0, 3, 0);
	mainLayout->addSpacing(15);
	mainLayout->addLayout(addLayout);
	mainLayout->addSpacing(5);
	mainLayout->addWidget(scrollArea);

	contentWidget->setLayout(layout);
	scrollArea->setWidget(contentWidget);
	scrollArea->setWidgetResizable(true);
//	scrollArea->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
//	scrollArea->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
	scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//	scrollArea->setMaximumWidth(450);
	scrollArea->setMinimumWidth(280);

	setLayout(mainLayout);

	connect(action, &QAction::triggered, this, &PropertyListWidget::addNewFloatProperty);
	connect(actionInt, &QAction::triggered, this, &PropertyListWidget::addNewIntProperty);
	connect(action2, &QAction::triggered, this, &PropertyListWidget::addNewVec2Property);
	connect(action3, &QAction::triggered, this, &PropertyListWidget::addNewVec3Property);
	connect(action4, &QAction::triggered, this, &PropertyListWidget::addNewVec4Property);
	connect(action5, &QAction::triggered, this, &PropertyListWidget::addNewTextureProperty);

	scrollArea->setStyleSheet(""
		"QScrollBar:vertical {border : 0px solid black;	background: rgba(132, 132, 132, 0);width: 10px; }"
		"QScrollBar::handle{ background: rgba(72, 72, 72, 1);	border-radius: 5px;  left: 8px; }"
		"QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {	background: rgba(200, 200, 200, 0);}"
		"QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical {	background: rgba(0, 0, 0, 0);border: 0px solid white;}"
		"QScrollBar::sub-line, QScrollBar::add-line {	background: rgba(10, 0, 0, .0);}"
	);

	pushButton->setStyleSheet(
		"QPushButton{ background: #333; color: #fefefe; border : 0; padding: 4px 16px; }"
		"QPushButton:hover{ background-color: #555; }"
		"QPushButton:pressed{ background-color: #444; }"
		"QPushButton:disabled{ color: #444; }"
		"QPushButton:checked{ background-color: rgba(50,150,255,1); }"
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
	added = 0; 

    // build properties
	for (auto prop : graph->properties) {

		switch (prop->type) {
		case PropertyType::Int:
			addIntProperty((IntProperty*)prop);
			break;
		case PropertyType::Float:
			addFloatProperty((FloatProperty*)prop);
			break;
		case PropertyType::Vec2:
			addVec2Property((Vec2Property*)prop);
			break;
		case PropertyType::Vec3:
			addVec3Property((Vec3Property*)prop);
			break;
		case PropertyType::Vec4:
			addVec4Property((Vec4Property*)prop);
			break;
		case PropertyType::Texture:
			addTextureProperty((TextureProperty*)prop, true);
			break;
		default:
			break;
		}
    }
}

void PropertyListWidget::clearPropertyList()
{
    // remove all QWidgets
    for(auto child : referenceList){
        referenceList.removeOne(child);
        layout->removeWidget(child);
        child->hide();
        child->deleteLater();
    }

	added = 0;
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
	propWidget->setProp(floatProp);
    addToPropertyListWidget(propWidget);
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
	propWidget->setProp(vec2Prop);
    addToPropertyListWidget(propWidget);

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
	propWidget->setProp(vec3Prop);
    addToPropertyListWidget(propWidget);
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
	propWidget->setProp(vec4Prop);
    addToPropertyListWidget(propWidget);
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
	propWidget->setProp(intProp);
    addToPropertyListWidget(propWidget);
}

////

void PropertyListWidget::addNewTextureProperty()
{
	auto prop = new TextureProperty;
	prop->displayName = "Texture Property";
	prop->name = QString("property%1").arg(graph->properties.count());
	this->addTextureProperty(prop);
	this->graph->addProperty(prop);
}

void PropertyListWidget::addTextureProperty(TextureProperty * texProp, bool requestTextureFromDatabase)
{
	auto propWidget = new TexturePropertyWidget;
	QString imagePath = emit imageRequestedForTexture(texProp->value);
	propWidget->setProp(texProp);
    addToPropertyListWidget(propWidget);
}

void PropertyListWidget::addToPropertyListWidget(BasePropertyWidget *widget)
{

    layout->insertWidget(layout->count() -1, widget); // minus one to account for stretch
    connect(widget, &BasePropertyWidget::currentWidget, [=](BasePropertyWidget *wid) {
        currentWidget = wid;
    });
    connect(widget, &BasePropertyWidget::TitleChanged, [=](QString title) {
		widget->modelProperty->displayName = title;
        emit nameChanged(title, widget->modelProperty->id);
    });

	connect(widget, &BasePropertyWidget::buttonPressed, [=](bool shouldDelete) {
		if (shouldDelete) emit deleteProperty(widget->modelProperty->id);
	});

    referenceList.append(widget);
    widget->index = added;
	added++;
    qDebug() << layout->count();
}
