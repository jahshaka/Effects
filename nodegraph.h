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
#include <QPointf>

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
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR) override;
};


class Socket : public QGraphicsPathItem
{
public:
    // note: in sockets can only have one connection
    QVector<SocketConnection*> connections;
    SocketType socketType;
    float radius;
    float dimentions;
    QGraphicsTextItem* text;
    GraphNode* node;
    GraphNode* owner;

    int socketIndex = -1;

    Socket(QGraphicsItem* parent, SocketType socketType, QString title);
    void addConnection(SocketConnection* con);
    float calcHeight();
    float getRadius();
    QPointF getPos();
    float getSocketOffset();
    virtual int type() const override;

    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
private:
    QPointF socketPos;
    QColor socketColor;
    QColor connectedColor = QColor(50,150,250);
    QColor disconnectedColor= QColor(90,90,90).darker(175);
    bool connected;
    bool rounded = true;

    QColor getSocketColor();
    void setSocketColor(QColor color);
    void setConnected(bool value);
    void updateSocket();

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
	bool isHighlighted = false;
	bool currentSelectedState = false;
	bool check = false;
    QString nodeId;
	QColor titleColor;

    GraphNode(QGraphicsItem* parent);

	void setTitleColor(QColor color);
    void setTitle(QString title);
    void addInSocket(QString title);
    void addOutSocket(QString title);
    void addSocket(Socket* sock);
    void setWidget(QWidget* widget);
    void calcPath();
    int calcHeight();

    Socket* getInSocket(int index);
    Socket* getOutSocket(int index);

    virtual void paint(QPainter *painter,
                const QStyleOptionGraphicsItem *option,
                QWidget *widget = 0) override;

    virtual int type() const override;
private:
    QColor connectedColor = QColor(50,150,250);
    QColor disconnectedColor= QColor(90,90,90,0);
    void highlightNode(bool val);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event)override;
};


class NodeGraph;
class NodeModel;
class Property;
class GraphNodeScene : public QGraphicsScene
{
    Q_OBJECT

    //QVector<GraphNode> nodes;

    // only used when dragging
    SocketConnection* con;

    // model for scene
    NodeGraph* nodeGraph;

	QGraphicsItemGroup *conGroup;
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
    SocketConnection* addConnection(QString leftNodeId, int leftSockIndex, QString rightNodeId, int rightSockIndex);
    SocketConnection* addConnection(Socket* leftCon, Socket* rightCon);


    bool eventFilter(QObject *o, QEvent *e);
    Socket* getSocketAt(float x, float y);
    GraphNode* getNodeById(QString id);
    GraphNode* getNodeByPos(QPointF point);

    NodeGraph *getNodeGraph() const;
    void setNodeGraph(NodeGraph* value);
    void addNodeModel(NodeModel* model, float x, float y, bool addToGraph = true);
    void addPropertyNode(Property* prop, float x, float y, bool addToGraph = true);

    QMenu* createContextMenu(float x, float y);

    QJsonObject serialize();

protected:
	void wheelEvent(QGraphicsSceneWheelEvent *event)override;
	void drawItems(QPainter *painter, int numItems,
		QGraphicsItem *items[],
		const QStyleOptionGraphicsItem options[],
		QWidget *widget = Q_NULLPTR);
	void dropEvent(QGraphicsSceneDragDropEvent *event) override;
	void drawBackground(QPainter *painter, const QRectF &rect);

signals:
    void newConnection(SocketConnection* connection);
    void connectionRemoved(SocketConnection* connection);
    void nodeValueChanged(NodeModel* nodeModel, int socketIndex);
};


#endif // NODEGRAPH_H
