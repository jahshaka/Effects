#ifndef NODEMODEL_H
#define NODEMODEL_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QMap>
#include <QUuid>
#include <QJsonValue>
#include <functional>
#include "properties.h"


class NodeModel;
class ConnectionModel;

class NodeGraph
{
public:
    QMap<QString, NodeModel*> nodes;
    QMap<QString, ConnectionModel*> connections;
    NodeModel* masterNode = nullptr;
    QList<Property*> properties;


    void addProperty(Property* prop);

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

    QJsonObject serialize();
    void deserialize(QJsonObject obj);
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

    virtual QString convertVarTo(SocketModel* toModel)
    {
        return varName;
    }

    virtual QString convertValueTo(SocketModel* toModel)
    {
        return value;
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


class SocketHelper
{
public:
    static QString convertVectorValue(QString fromValue, SocketModel* from, SocketModel* to)
    {
        int numFrom = getNumComponents(from->typeName);
        int numTo = getNumComponents(to->typeName);

        Q_ASSERT(numFrom!= 0 && numTo != 0);

        QString suffix = ".";

        // float being casted to a vector
        //
        if (numFrom==1 && numTo>1) {
            return QString("vec%1(%2)").arg(numTo).arg(fromValue);
        }

        if (numFrom > numTo) {
            // eg vec4 > vec2 = var.xy
            for(int i = 0; i<numTo; i++) {
                suffix += getVectorComponent(i);
            }

            return fromValue+suffix;
        }
        if (numFrom < numTo)
        {
            // eg vec2 > vec4 = var.xyyy
            for(int i = 0; i<numTo; i++) {
                suffix += getVectorComponent(qMin(i, numTo-2));
            }
            return fromValue+suffix;
        }

        return fromValue;

    }

    static QString getVectorComponent(int index)
    {
        switch(index)
        {
            case 0: return "x";
            case 1: return "y";
            case 2: return "z";
            case 3: return "w";
        }

        return "x";
    }

    static int getNumComponents(QString valueType)
    {
        if (valueType=="float")
            return 1;

        if (valueType=="vec2")
            return 2;

        if (valueType=="vec3")
            return 3;

        if (valueType=="vec4")
            return 4;

        return 0;
    }
};

class FloatSocketModel : public SocketModel
{
public:
    FloatSocketModel(QString name, QString defaultValue = "0.0f"):
        SocketModel(name,"float")
    {
        this->setValue(defaultValue);
    }

    QString convertVarTo(SocketModel* toModel)
    {
        return SocketHelper::convertVectorValue(varName, this, toModel);
    }

    virtual SocketModel* duplicate()
    {
        return new FloatSocketModel(name);
    }
};

class Vector2SocketModel : public SocketModel
{
public:
    Vector2SocketModel(QString name, QString defaultValue = "vec2(0.0f, 0.0f)"):
        SocketModel(name, "vec2")
    {
        this->setValue(defaultValue);
    }

    QString convertVarTo(SocketModel* toModel)
    {
        return SocketHelper::convertVectorValue(varName, this, toModel);
    }

    virtual SocketModel* duplicate()
    {
        return new Vector2SocketModel(name);
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

    QString convertVarTo(SocketModel* toModel)
    {
        return SocketHelper::convertVectorValue(varName, this, toModel);
    }

    virtual SocketModel* duplicate()
    {
        return new Vector3SocketModel(name);
    }
};

class Vector4SocketModel : public SocketModel
{
public:
    Vector4SocketModel(QString name, QString defaultValue = "vec4(0.0f, 0.0f, 0.0f, 0.0f)"):
        SocketModel(name, "vec4")
    {
        this->setValue(defaultValue);
    }

    QString convertVarTo(SocketModel* toModel)
    {
        return SocketHelper::convertVectorValue(varName, this, toModel);
    }

    virtual SocketModel* duplicate()
    {
        return new Vector4SocketModel(name);
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

    virtual QJsonValue serializeWidgetValue(int widgetIndex = 0)
    {
        return "";
    }

    virtual void deserializeWidgetValue( QJsonValue val, int widgetIndex = 0)
    {

    }

signals:
    void valueChanged(NodeModel*, int sockedIndex);

protected:
    virtual NodeModel* createDuplicate()
    {
        return new NodeModel();
    }
};

#endif // NODEMODEL_H
