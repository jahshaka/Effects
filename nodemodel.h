#ifndef NODEMODEL_H
#define NODEMODEL_H

#include <QVector>
#include <QString>
#include <QMap>
#include <QUuid>


class NodeModel;
class ConnectionModel;

class NodeGraph
{
public:
    QMap<QString, NodeModel*> nodes;
    QMap<QString, ConnectionModel*> connections;

    NodeModel* masterNode;

    QMap<QString, NodeModel*> modelRegistry;
    void registerModel(NodeModel* model);

    void addNode(NodeModel* model);

    void setMasterNode(NodeModel* masterNode);
    NodeModel* getMasterNode();

    void addConnection(NodeModel* leftNode, int leftSockIndex, NodeModel* rightNode, int rightSockIndex);

    void addConnection(QString leftNodeId, int leftSockIndex, QString rightNodeId, int rightSockIndex);

    // gets the output node and socket for a given input node and socket
    ConnectionModel* getConnectionFromOutputNode(NodeModel* node, int socketIndex);
};

class ConnectionModel
{
public:
    QString id;

    QString leftNodeId;
    int leftSocketIndex;

    QString rightNodeId;
    int rightSocketIndex;

    ConnectionModel();
};

class ModelContext
{
public:
    virtual QString getTempVarName(QString typeName);
};

class SocketModel
{
public:
    QString id;
    QString name;
    QString typeName;
    QString defaultValue;

    SocketModel();
    SocketModel(QString name, QString typeName);

    virtual QString getDefaultValue()
    {
        return defaultValue;
    }

    virtual bool canConvertTo(SocketModel* other)
    {
        // todo: make false by default
        return true;
    }

    virtual SocketModel* duplicate() = 0;
};

class FloatSocketModel : public SocketModel
{
public:
    FloatSocketModel(QString name, QString defaultValue = "0.0f"):
        SocketModel(name,"float")
    {
        this->defaultValue = defaultValue;
    }

    virtual SocketModel* duplicate()
    {
        return new FloatSocketModel(name);
    }
};

class Vector3SocketModel : public SocketModel
{
public:
    Vector3SocketModel(QString name, QString defaultValue = "vec3(0.0f, 0.0f, 0.0f)"):
        SocketModel(name, "vec3")
    {
        this->defaultValue = defaultValue;
    }

    virtual SocketModel* duplicate()
    {
        return new Vector3SocketModel(name);
    }
};

class NodeModel
{
public:
    QString id;

    QVector<SocketModel*> inSockets;
    QVector<SocketModel*> outSockets;

    QString typeName;
    QString title;

    NodeModel();

    virtual QString getSocketValue(int socketIndex, ModelContext* context){}

    // for the master node
    void calculate(ModelContext* context);

    virtual NodeModel* duplicate();
};

#endif // NODEMODEL_H
