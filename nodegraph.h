#ifndef NODEGRAPH_H
#define NODEGRAPH_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsPathItem>
#include <QGraphicsTextItem>
#include <QGraphicsItem>
#include <QFontMetrics>
#include <QtMath>
#include <QGraphicsSceneMouseEvent>
#include <QJsonObject>

enum class SocketType
{
    In,
    Out
};

enum class GraphicsItemType : int
{
    Node = QGraphicsItem::UserType + 1,
    Socket = QGraphicsItem::UserType + 2,
    Connection = QGraphicsItem::UserType + 3
};

class Socket;
class GraphNode;

class SocketConnection : public QGraphicsPathItem
{
public:
    QString connectionId;

    Socket* socket1;
    Socket* socket2;

    QPointF pos1;
    QPointF pos2;

    SocketConnection();

    void updatePosFromSockets();
    void updatePath();
    virtual int type() const override;
};


class Socket : public QGraphicsPathItem
{
public:
    // note: in sockets can only have one connection
    QVector<SocketConnection*> connections;
    SocketType socketType;
    float radius;
    QGraphicsTextItem* text;
    GraphNode* node;

    int socketIndex = -1;

    Socket(QGraphicsItem* parent, SocketType socketType, QString title);
    void addConnection(SocketConnection* con);
    float calcHeight();
    float getRadius();
    virtual int type() const override;

    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};

class GraphNode : public QGraphicsPathItem
{
    QVector<Socket*> sockets;
    int nodeWidth;
    QGraphicsTextItem* text;
    QGraphicsProxyWidget* proxyWidget;

    int inSocketCount = 0;
    int outSocketCount = 0;
public:
    int nodeType;
    QString nodeId;

    GraphNode(QGraphicsItem* parent);

    void setTitle(QString title);
    void addInSocket(QString title);
    void addOutSocket(QString title);
    void addSocket(Socket* sock);
    void setWidget(QWidget* widget);
    void calcPath();
    int calcHeight();

    virtual void paint(QPainter *painter,
                const QStyleOptionGraphicsItem *option,
                QWidget *widget = 0) override;

    virtual int type() const override;
};


class NodeGraph;
class NodeModel;
class GraphNodeScene : public QGraphicsScene
{
    Q_OBJECT

    QVector<GraphNode> nodes;

    // only used when dragging
    SocketConnection* con;

    // model for scene
    NodeGraph* nodeGraph;
public:
    GraphNodeScene(QWidget* parent);

    template<class nodeType>
    GraphNode* createNode()
    {
    //        static_assert(
    //                std::is_base_of<MyBase, T>::value,
    //                "T must be a descendant of MyBase"
    //            );
        auto node = new nodeType(nullptr);
        this->addItem(node);

        return node;
    }

    bool eventFilter(QObject *o, QEvent *e);
    Socket* getSocketAt(float x, float y);

    NodeGraph *getNodeGraph() const;
    void setNodeGraph(NodeGraph *value);
    void addNodeModel(NodeModel* model, float x, float y, bool addToGraph = true);

    QMenu* createContextMenu(float x, float y);

    QJsonObject serialize();

signals:
    void newConnection(SocketConnection* connection);
    void connectionRemoved(SocketConnection* connection);
    void nodeValueChanged(NodeModel* nodeModel, int socketIndex);
};


#endif // NODEGRAPH_H
