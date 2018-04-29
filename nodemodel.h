#ifndef NODEMODEL_H
#define NODEMODEL_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QMap>
#include <QUuid>
#include <functional>


class NodeModel;
class ConnectionModel;

class NodeGraph
{
public:
    QMap<QString, NodeModel*> nodes;
    QMap<QString, ConnectionModel*> connections;

    NodeModel* masterNode = nullptr;

    QMap<QString, std::function<NodeModel*()>> modelFactories;
    void registerModel(QString name, std::function<NodeModel*()> factoryFunction);

    void addNode(NodeModel* model);

    // master node must already be added as a node
    void setMasterNode(NodeModel* masterNode);
    NodeModel* getMasterNode();

    void addConnection(NodeModel* leftNode, int leftSockIndex, NodeModel* rightNode, int rightSockIndex);
    void addConnection(QString leftNodeId, int leftSockIndex, QString rightNodeId, int rightSockIndex);

    // gets the output node and socket for a given input node and socket
    ConnectionModel* getConnectionFromOutputNode(NodeModel* node, int socketIndex);
};

class SocketModel;
class ConnectionModel
{
public:
    QString id;

    SocketModel* leftSocket;
    SocketModel* rightSocket;

    ConnectionModel();
};

class ModelContext
{
};

class SocketModel
{
public:
    QString id;
    QString name;
    QString typeName; //todo: change to enum

    // used to store results of calculations
    // usually only right nodes have these assigned
    QString varName;
    // used to get calculation results
    // will sometimes be var name
    QString value;

    // connection if any
    ConnectionModel* connection = nullptr;
    NodeGraph* graph = nullptr;

    NodeModel* node = nullptr;

    SocketModel();
    SocketModel(QString name, QString typeName);

    virtual bool canConvertTo(SocketModel* other)
    {
        // todo: make false by default
        return true;
    }

    virtual SocketModel* duplicate() = 0;

    void setGraph(NodeGraph *value);
    NodeGraph *getGraph() const;

    // assigned by shader model context
    QString getValue() const;
    void setValue(const QString &value);

    QString getVarName() const;
    void setVarName(const QString &value);

    bool hasConnection()
    {
        return connection != nullptr;
    }
    ConnectionModel* getConnection()
    {
        return connection;
    }

    SocketModel* getConnectedSocket();
    NodeModel *getNode() const;
    void setNode(NodeModel *value);
};


class FloatSocketModel : public SocketModel
{
public:
    FloatSocketModel(QString name, QString defaultValue = "0.0f"):
        SocketModel(name,"float")
    {
        this->setValue(defaultValue);
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
        this->setValue(defaultValue);
    }

    virtual SocketModel* duplicate()
    {
        return new Vector3SocketModel(name);
    }
};

class QWidget;
class NodeModel : public QObject
{
    Q_OBJECT
public:
    QString id;

    QVector<SocketModel*> inSockets;
    QVector<SocketModel*> outSockets;

    QString typeName;
    QString title;

    QWidget* widget;

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


    virtual void preProcess(ModelContext* context){}
    virtual void process(ModelContext* context){}
    virtual void postProcess(ModelContext* context){}

    NodeGraph *getGraph() const;
    void setGraph(NodeGraph *value);

signals:
    void valueChanged(NodeModel*, int sockedIndex);

protected:
    virtual NodeModel* createDuplicate()
    {
        return new NodeModel();
    }
};

#endif // NODEMODEL_H
