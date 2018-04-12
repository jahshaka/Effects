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
#include "nodegraph.h"
#include "nodemodel.h"


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

    return height;
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

void GraphNodeScene::setNodeGraph(NodeGraph *value)
{
    // clear previous nodegraph
    // recreate nodes
    nodeGraph = value;

    auto masterNode = nodeGraph->getMasterNode();
    addNodeModel(masterNode, 0, 0, false);
}

void GraphNodeScene::addNodeModel(NodeModel *model, float x, float y, bool addToGraph)
{
    auto nodeView = this->createNode<GraphNode>();
    for(auto sock : model->inSockets)
        nodeView->addInSocket(sock->name);
    for(auto sock : model->outSockets)
        nodeView->addOutSocket(sock->name);
    nodeView->setTitle(model->typeName);

    nodeView->setPos(x, y);
    nodeView->nodeId = model->id;

    if (addToGraph) {
        Q_ASSERT_X(nodeGraph!=nullptr,"GraphNodeScene::addNodeModel", "Cant add node to null scene");
        nodeGraph->addNode(model);
    }
}

QMenu *GraphNodeScene::createContextMenu(float x, float y)
{
    auto menu = new QMenu();

    for(auto model : nodeGraph->modelRegistry.values()) {
        connect(menu->addAction(model->title), &QAction::triggered, [this,x, y,model](){

            auto multNode = model->duplicate();
            this->addNodeModel(multNode, x, y);

        });
    }

    return menu;
}

GraphNodeScene::GraphNodeScene(QWidget* parent):
    QGraphicsScene(parent)
{
    con = nullptr;
    nodeGraph = nullptr;
    this->installEventFilter(this);
}

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
