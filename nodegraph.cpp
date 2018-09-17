#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsPathItem>
#include <QGraphicsTextItem>
#include <QGraphicsItem>
#include <QFontMetrics>
#include <QtMath>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QWidget>
#include <QMenu>
#include <QGraphicsWidget>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include "nodegraph.h"
#include "nodemodel.h"
#include "graphtest.h"


Socket::Socket(QGraphicsItem* parent, SocketType socketType, QString title):
    QGraphicsPathItem(parent),
    socketType(socketType)
{
    this->setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
    text = new QGraphicsTextItem(this);
    text->setPlainText(title);

    QFontMetrics fm(parent->scene()->font());
    QRect textRect = fm.boundingRect(title);

    radius = qCeil(textRect.height()/2.0f);
    QPainterPath path;
    path.addRect(-radius, -radius, radius * 2, radius * 2);
    setBrush(QBrush(QColor(200, 250, 250)));
    setPen(Qt::NoPen);
    setPath(path);

    // socket positions are at the outer right or outer left of the graph node
    if (socketType == SocketType::Out)
    {
        // left node
        text->setPos(-textRect.width() -radius - 5, -radius);
    }
    else
    {
        text->setPos(radius, -radius);
    }
}

void Socket::addConnection(SocketConnection* con)
{
    connections.append(con);
}

float Socket::calcHeight()
{
    return radius * 2;
}

float Socket::getRadius()
{
    return radius;
}

int Socket::type() const
{
    return (int)GraphicsItemType::Socket;
}

QVariant Socket::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemScenePositionHasChanged)
    {
        for(auto con : connections)
        {
            con->updatePosFromSockets();
            con->updatePath();
        }
    }
    return value;
}

SocketConnection::SocketConnection()
{
    socket1 = nullptr;
    socket2 = nullptr;

    pos1 = QPointF(0,0);
    pos2 = QPointF(0,0);

    auto pen = QPen(QColor(200, 200, 200));
    pen.setWidth(5);
    setPen(pen);
}

void SocketConnection::updatePosFromSockets()
{
    pos1 = socket1->scenePos();
    pos2 = socket2->scenePos();
}

void SocketConnection::updatePath()
{
    QPainterPath p;

    p.moveTo(pos1);

    qreal dx = pos2.x() - pos1.x();
    qreal dy = pos2.y() - pos1.y();

    QPointF ctr1(pos1.x() + dx * 0.25, pos1.y() + dy * 0.1);
    QPointF ctr2(pos1.x() + dx * 0.75, pos1.y() + dy * 0.9);

    p.cubicTo(ctr1, ctr2, pos2);

    setPath(p);
}

int SocketConnection::type() const
{
    return (int)GraphicsItemType::Connection;
}


GraphNode::GraphNode(QGraphicsItem* parent):
    QGraphicsPathItem(parent)
{
    nodeType = 0;
    proxyWidget = nullptr;

    this->setFlag(QGraphicsItem::ItemIsMovable);
    this->setFlag(QGraphicsItem::ItemIsSelectable);
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    this->setFlag(QGraphicsItem::ItemSendsScenePositionChanges);

    nodeWidth = 150;
    QPainterPath path_content;
    path_content.setFillRule(Qt::WindingFill);
    //path_content.addRoundedRect(QRect(0, 0, 100, 220), 5, 5);
    path_content.addRect(QRect(0, 0, nodeWidth, calcHeight()));
    setPath(path_content);

    setPen(QPen(Qt::black));
    setBrush(QColor(240, 240, 240));

    text = new QGraphicsTextItem(this);
    text->setPlainText("Title");
    text->setPos(5, 5);
    text->setDefaultTextColor(QColor(255, 255, 255));
}

void GraphNode::setTitle(QString title)
{
    text->setPlainText(title);
}

void GraphNode::addInSocket(QString title)
{
    auto sock = new Socket(this, SocketType::In, title);
    auto y = calcHeight();// + sock->calcHeight();
    sock->setPos(-sock->getRadius(), y);
    sock->node = this;
    sock->socketIndex = inSocketCount++;
    addSocket(sock);
}

void GraphNode::addOutSocket(QString title)
{
    auto sock = new Socket(this, SocketType::Out, title);
    auto y = calcHeight();// + sock->calcHeight();
    sock->setPos(nodeWidth + sock->getRadius(), y);
    sock->node = this;
    sock->socketIndex = outSocketCount++;
    addSocket(sock);
}

void GraphNode::addSocket(Socket* sock)
{
    sockets.append(sock);

    // recalc path
    calcPath();
}

void GraphNode::setWidget(QWidget *widget)
{
    // gotta do this here before adding the widget
    auto y = calcHeight();

    proxyWidget = new QGraphicsProxyWidget(this);
    proxyWidget->setWidget(widget);
    //proxyWidget->setPreferredSize(widget->pr);
    proxyWidget->setPreferredWidth(5);
    proxyWidget->setPos((nodeWidth-proxyWidget->size().width()) / 2,
                        y);

    calcPath();
}

void GraphNode::calcPath()
{
    QPainterPath path_content;
    path_content.setFillRule(Qt::WindingFill);
    path_content.addRect(QRect(0, 0, nodeWidth, calcHeight()));
    setPath(path_content);
}

int GraphNode::calcHeight()
{
    int height = 0;
    height += 35 + 5;// title + padding

    for(auto socket : sockets)
    {
        height += socket->calcHeight();
        height += 5; // padding
    }

    //height += 2; // padding
    if (proxyWidget != nullptr)
        height += proxyWidget->size().height();

    return height;
}

Socket *GraphNode::getInSocket(int index)
{
    int i = 0;
    for(auto sock : sockets) {
        if (sock->socketType == SocketType::In) {
            if (index == i)
                return sock;
            i ++;
        }
    }

    return nullptr;
}

Socket *GraphNode::getOutSocket(int index)
{
    int i = 0;
    for(auto sock : sockets) {
        if (sock->socketType == SocketType::Out) {
            if (index == i)
                return sock;
            i ++;
        }
    }
}

void GraphNode::paint(QPainter *painter,
            const QStyleOptionGraphicsItem *option,
            QWidget *widget)
{
    painter->setPen(pen());
    painter->fillPath(path(), brush());

    // title tab
    QPainterPath titlePath;
    titlePath.addRect(0, 0, nodeWidth, 35);
    painter->fillPath(titlePath, QBrush(QColor(135, 206, 235)));
}

int GraphNode::type() const
{
    return (int)GraphicsItemType::Node;
}


NodeGraph *GraphNodeScene::getNodeGraph() const
{
    return nodeGraph;
}

void GraphNodeScene::setNodeGraph(NodeGraph *graph)
{
    // clear previous nodegraph

    // recreate nodes
    nodeGraph = graph;

    //auto masterNode = nodeGraph->getMasterNode();
    //addNodeModel(masterNode, 0, 0, false);

    // add nodes
    for(auto node : graph->nodes.values()) {
        this->addNodeModel(node, 0, 0, false);
    }

    // add connections
    for(auto con : graph->connections.values()) {
        auto leftNode = con->leftSocket->node;
        auto rightNode = con->rightSocket->node;
        this->addConnection(leftNode->id,
                            leftNode->outSockets.indexOf(con->leftSocket),
                            rightNode->id,
                            rightNode->inSockets.indexOf(con->rightSocket));
    }
}

// add
void GraphNodeScene::addNodeModel(NodeModel *model, float x, float y, bool addToGraph)
{
    auto nodeView = this->createNode<GraphNode>();
    for(auto sock : model->inSockets)
        nodeView->addInSocket(sock->name);
    for(auto sock : model->outSockets)
        nodeView->addOutSocket(sock->name);

    if (model->widget != nullptr)
        nodeView->setWidget(model->widget);
    nodeView->setTitle(model->title);

    nodeView->setPos(x, y);
    nodeView->nodeId = model->id;

    if (addToGraph) {
        Q_ASSERT_X(nodeGraph!=nullptr,"GraphNodeScene::addNodeModel", "Cant add node to null scene");
        nodeGraph->addNode(model);
    }

    connect(model, &NodeModel::valueChanged, [this](NodeModel* nodeModel, int sockedIndex){
        emit nodeValueChanged(nodeModel, sockedIndex);
    });
}

QMenu *GraphNodeScene::createContextMenu(float x, float y)
{
    auto menu = new QMenu();

    for(auto key : nodeGraph->modelFactories.keys()) {
        auto factory = nodeGraph->modelFactories[key];
        connect(menu->addAction(key), &QAction::triggered, [this,x, y,factory](){

            auto node = factory();
            this->addNodeModel(node, x, y);

        });
    }

    // create properties
    auto propMenu = menu->addMenu("Properties");
    for(auto prop:nodeGraph->properties) {
        connect(propMenu->addAction(prop->displayName), &QAction::triggered, [this,x, y,prop](){
            auto propNode = new PropertyNode();
            propNode->setProperty(prop);
            this->addNodeModel(propNode, x, y);
        });
    }

    return menu;
}

QJsonObject GraphNodeScene::serialize()
{
    QJsonObject data;

    data["graph"] = this->nodeGraph->serialize();
    QJsonObject scene;

    QJsonArray nodesJson;

    // save nodes

    for(auto item : this->items()) {
        if (item && item->type() == (int)GraphicsItemType::Node){
            auto node = (GraphNode*)item;
            QJsonObject nodeObj;
            nodeObj["id"] = node->nodeId;
            nodeObj["x"] = node->x();
            nodeObj["y"] = node->y();
            nodesJson.append(nodeObj);
        }
    }

    scene.insert("nodes", nodesJson);
    data["scene"] = scene;

    return data;
}

GraphNodeScene::GraphNodeScene(QWidget* parent):
    QGraphicsScene(parent)
{
    con = nullptr;
    nodeGraph = nullptr;
    this->installEventFilter(this);
}

SocketConnection *GraphNodeScene::addConnection(QString leftNodeId, int leftSockIndex, QString rightNodeId, int rightSockIndex)
{
    auto leftNode = this->getNodeById(leftNodeId);
    auto rightNode = this->getNodeById(rightNodeId);

    Q_ASSERT(leftNode != nullptr);
    Q_ASSERT(rightNode != nullptr);

    auto con = new SocketConnection();
    con->socket1 = leftNode->getOutSocket(leftSockIndex);
    con->socket2 = rightNode->getInSocket(rightSockIndex);
    con->socket1->addConnection(con);
    con->socket2->addConnection(con);
    con->updatePosFromSockets();
    con->updatePath();
    this->addItem(con);

	return con;
}
/*
SocketConnection* GraphNodeScene::addConnection(Socket* leftCon, Socket* rightCon)
{
    auto leftNode = this->getNodeById(leftNodeId);
    auto rightNode = this->getNodeById(rightNodeId);

    Q_ASSERT(leftNode != nullptr);
    Q_ASSERT(rightNode != nullptr);

    auto con = new SocketConnection();
    con->socket1 = leftNode->getOutSocket(leftSockIndex);
    con->socket2 = rightNode->getInSocket(rightSockIndex);
    con->socket1->addConnection(con);
    con->socket2->addConnection(con);
    con->updatePosFromSockets();
    con->updatePath();
    this->addItem(con);
}
*/
bool GraphNodeScene::eventFilter(QObject *o, QEvent *e)
{
    QGraphicsSceneMouseEvent *me = (QGraphicsSceneMouseEvent*) e;

    switch ((int) e->type())
    {
    case QEvent::GraphicsSceneMousePress:
    {
        auto sock = getSocketAt(me->scenePos().x(), me->scenePos().y());
        if (sock != nullptr) {

            con = new SocketConnection();
            con->socket1 = sock;
            con->pos1 = me->scenePos();
            con->pos2 = me->scenePos();
            con->updatePath();
            this->addItem(con);

            return true;
        }
        else if (me->button() == Qt::RightButton)
        {

            auto x = me->scenePos().x();
            auto y = me->scenePos().y();

            auto menu = createContextMenu(x, y);

            auto view = this->views().first();
            auto scenePoint = view->mapFromScene(me->scenePos());
            auto p = view->viewport()->mapToGlobal(scenePoint);

            menu->exec(p);
        }

    }
    break;
    case QEvent::GraphicsSceneMouseMove:
    {
        if (con) {
            con->pos2 = me->scenePos();
            con->updatePath();
            return true;
        }
    }
    break;
    case QEvent::GraphicsSceneMouseRelease:
    {
        if (con) {
            // make it an official connection
            auto sock = getSocketAt(me->scenePos().x(), me->scenePos().y());
            if (sock != nullptr) {
                // ensure only outs can connect to ins
                if (sock->socketType != con->socket1->socketType) {
                    // todo: prevent recursive connection
                    if (con->socket1->node != sock->node) {// prevent it connecting to itself
                        con->socket2 = sock;

                        con->socket1->addConnection(con);
                        con->socket2->addConnection(con);
                        con->updatePosFromSockets();
                        con->updatePath();

                        // connect models too
                        if (nodeGraph != nullptr) {
                            // check the order of nodes
                            if (con->socket1->socketType ==SocketType::Out) {
                                this->nodeGraph->addConnection(con->socket1->node->nodeId,con->socket1->socketIndex,
                                                               con->socket2->node->nodeId,con->socket2->socketIndex);
                            } else {
                                this->nodeGraph->addConnection(con->socket2->node->nodeId,con->socket2->socketIndex,
                                                               con->socket1->node->nodeId,con->socket1->socketIndex);
                            }
                        }

                        emit newConnection(con);
                        con = nullptr;

                        return true;
                    }
                }
            }

            this->removeItem(con);
            delete con;
            con = nullptr;

            return true;
        }
    }
    break;
    }

    return QObject::eventFilter(o, e);
}

Socket* GraphNodeScene::getSocketAt(float x, float y)
{
    auto items = this->items(QPointF(x, y));
    //auto items = this->items();
    for (auto item : items) {
        if (item && item->type() == (int)GraphicsItemType::Socket)
            return (Socket*)item;
    }

    return nullptr;
}

GraphNode *GraphNodeScene::getNodeById(QString id)
{
    auto items = this->items();
    for (auto item : items) {
        if (item && item->type() == (int)GraphicsItemType::Node)
			if (((GraphNode*)item)->nodeId == id)
				return (GraphNode*)item;
    }

    return nullptr;
}
