#ifndef PROPERTYLISTWIDGET_H
#define PROPERTYLISTWIDGET_H

#include <QWidget>

namespace Ui {
class PropertyListWidget;
}

class QVBoxLayout;
class NodeGraph;
class FloatProperty;
class Vec2Property;

class PropertyListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PropertyListWidget(QWidget *parent = 0);
    ~PropertyListWidget();

    void addProperty(QWidget* widget);
    void setNodeGraph(NodeGraph* graph);

private:
    void addNewFloatProperty();
	void addFloatProperty(FloatProperty* floatProp);

	void addNewVec2Property();
	void addVec2Property(Vec2Property* vec2Prop);

private:
    Ui::PropertyListWidget *ui;
    QVBoxLayout* layout;
    NodeGraph* graph;
};

#endif // PROPERTYLISTWIDGET_H
