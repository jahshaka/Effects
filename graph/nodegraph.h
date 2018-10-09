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
class NodeLibrary;


struct MaterialSettings {
	QString name;
	bool zwrite;
	bool depthTest;
	bool fog;
	bool castShadow;
	bool receiveShadow;
	bool acceptLighting;
	int blendMode;
	int cullMode;
	int renderLayer;
};

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
	MaterialSettings settings;

	void addProperty(Property* prop);
	Property* getPropertyByName(const QString& name);
	Property* getPropertyById(const QString& id);

	//QMap<QString, std::function<NodeModel*()>> modelFactories;
	//void registerModel(QString name, std::function<NodeModel*()> factoryFunction);
	NodeLibrary* library;
	void setNodeLibrary(NodeLibrary* lib);

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
	QJsonObject serializeMaterialSettings();
	MaterialSettings deserializeMaterialSettings(QJsonObject obj);

	void setMaterialSettings(MaterialSettings setting);
};

#endif// NODEGRAPH2_H