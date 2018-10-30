#ifndef NODE_MODEL_H
#define NODE_MODEL_H

#include <QObject>
#include <QJsonValue>
#include <QVector>
#include <QColor>
#include <QIcon>

#include "socketmodel.h"

enum class NodeType {
	Input,
	Properties,
	Math
};

class QWidget;
class ModelContext;

class NodeModel : public QObject
{
	Q_OBJECT
public:
	QString id;

	QVector<SocketModel*> inSockets;
	QVector<SocketModel*> outSockets;

	QString typeName;
	QString title;
	NodeType nodeType;

	QWidget* widget;
	QColor titleColor;
	QIcon icon;
	NodeModel();

	NodeGraph* graph = nullptr;

	void addInputSocket(SocketModel* sock);
	void addOutputSocket(SocketModel* sock);

	virtual QString getSocketValue(int socketIndex, ModelContext* context)
	{
		return outSockets[socketIndex]->getValue();
	}

	QString getValueFromInputSocket(int index);
	QString getOutputSocketVarName(int index);


	virtual void preProcess(ModelContext* context) {}
	virtual void process(ModelContext* context) {}
	virtual void postProcess(ModelContext* context) {}

	NodeGraph *getGraph() const;
	void setGraph(NodeGraph *value);
	QColor setNodeTitleColor();
	void setNodeType(NodeType type);


	virtual QJsonValue serializeWidgetValue(int widgetIndex = 0)
	{
		return "";
	}

	virtual void deserializeWidgetValue(QJsonValue val, int widgetIndex = 0)
	{

	}

signals:
	void valueChanged(NodeModel*, int sockedIndex);
	void titleColorChanged();

protected:
	virtual NodeModel* createDuplicate()
	{
		return new NodeModel();
	}
};

#endif// NODE_MODEL_H