#ifndef NODEGRAPH2_H
#define NODEGRAPH2_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QMap>
#include <QUuid>
#include <QJsonValue>
#include <QJsonObject>
#include <functional>
#include "properties.h"

class NodeModel;
class ConnectionModel;

class ModelContext
{
};

class NodeGraph
{
public:
	QMap<QString, NodeModel*> nodes;
	QMap<QString, ConnectionModel*> connections;
	NodeModel* masterNode = nullptr;
	QList<Property*> properties;


	void addProperty(Property* prop);
	Property* getPropertyByName(const QString& name);
	Property* getPropertyById(const QString& id);

	QMap<QString, std::function<NodeModel*()>> modelFactories;
	void registerModel(QString name, std::function<NodeModel*()> factoryFunction);

	void addNode(NodeModel* model);

	// master node must already be added as a node
	void setMasterNode(NodeModel* masterNode);
	NodeModel* getMasterNode();

	ConnectionModel* addConnection(NodeModel* leftNode, int leftSockIndex, NodeModel* rightNode, int rightSockIndex);
	ConnectionModel* addConnection(QString leftNodeId, int leftSockIndex, QString rightNodeId, int rightSockIndex);

	// gets the output node and socket for a given input node and socket
	ConnectionModel* getConnectionFromOutputNode(NodeModel* node, int socketIndex);

	QJsonObject serialize();
	static NodeGraph* deserialize(QJsonObject obj);
};

#endif// NODEGRAPH2_H