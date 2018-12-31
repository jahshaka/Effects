#include <QUuid>
#include "socketmodel.h"
#include "connectionmodel.h"
#include "../core/guidhelper.h"

void SocketModel::setGraph(NodeGraph *value)
{
	graph = value;
}

NodeGraph *SocketModel::getGraph() const
{
	return graph;
}

QString SocketModel::getVarName() const
{
	return varName;
}

void SocketModel::setVarName(const QString &value)
{
	varName = value;
}

SocketModel *SocketModel::getConnectedSocket()
{
	Q_ASSERT(connection != nullptr);
	Q_ASSERT(connection->leftSocket != nullptr && connection->rightSocket != nullptr);

	if (connection->leftSocket == this)
		return connection->rightSocket;

	if (connection->rightSocket == this)
		return connection->leftSocket;

	return nullptr;
}

QString SocketModel::getValue() const
{
	return value;
}

void SocketModel::setValue(const QString &value)
{
	this->value = value;
}

NodeModel *SocketModel::getNode() const
{
	return node;
}

void SocketModel::setNode(NodeModel *value)
{
	node = value;
}

SocketModel::SocketModel()
{
	id = QUuid::createUuid().toString();
}

SocketModel::SocketModel(QString name, QString typeName) :
	name(name),
	typeName(typeName)
{
	id = GuidHelper::createGuid();
}