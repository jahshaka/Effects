#include "graphnodescene.h"
#include "nodes/test.h"

#include <QMimeData>




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
	for (auto node : graph->nodes.values()) {
		this->addNodeModel(node, 0, 0, false);
	}

	// add connections
	for (auto con : graph->connections.values()) {
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
	nodeView->setTitle(model->title);
	nodeView->setTitleColor(model->setNodeTitleColor());
	//nodeView->setIcon(model->icon);

	for (auto sock : model->inSockets)
		nodeView->addInSocket(sock->name);
	for (auto sock : model->outSockets)
		nodeView->addOutSocket(sock->name);

	if (model->widget != nullptr)
		nodeView->setWidget(model->widget);
	/*nodeView->setTitle(model->title);
	nodeView->setTitleColor(model->setNodeTitleColor());*/

	nodeView->setPos(x, y);
	nodeView->nodeId = model->id;
	nodeView->layout();

	if (addToGraph) {
		Q_ASSERT_X(nodeGraph != nullptr, "GraphNodeScene::addNodeModel", "Cant add node to null scene");
		nodeGraph->addNode(model);
	}

	connect(model, &NodeModel::valueChanged, [this](NodeModel* nodeModel, int sockedIndex) {
		emit nodeValueChanged(nodeModel, sockedIndex);
	});
}

QMenu *GraphNodeScene::createContextMenu(float x, float y)
{
	auto menu = new QMenu();

	/*
	for(auto key : nodeGraph->modelFactories.keys()) {
	auto factory = nodeGraph->modelFactories[key];
	connect(menu->addAction(key), &QAction::triggered, [this,x, y,factory](){

	auto node = factory();
	this->addNodeModel(node, x, y);

	});
	}
	*/
	for (auto item : nodeGraph->library->getItems()) {
		auto factory = item->factoryFunction;
		connect(menu->addAction(item->displayName), &QAction::triggered, [this, x, y, factory]() {

			auto node = factory();
			this->addNodeModel(node, x, y);

		});
	}

	// create properties
	auto propMenu = menu->addMenu("Properties");
	for (auto prop : nodeGraph->properties) {
		connect(propMenu->addAction(prop->displayName), &QAction::triggered, [this, x, y, prop]() {
			auto propNode = new PropertyNode();
			propNode->setProperty(prop);
			this->addNodeModel(propNode, x, y);
		});
	}

	return menu;
}

QMenu * GraphNodeScene::removeConnectionContextMenu(float x, float y)
{
	auto menu = new QMenu();
	auto sock = getSocketAt(x, y);

	auto getAppropriateText = [&](SocketConnection *conn, int i) {
		if (i == 2) return conn->socket2->text->toPlainText();
		if (i == 1) return conn->socket1->text->toPlainText();
	};

	QString string;

	for (SocketConnection* connection : sock->connections) {
		string = QString("remove %1 - %2 connection").arg(getAppropriateText(connection, 1)).arg(getAppropriateText(connection, 2));
		connect(menu->addAction(string), &QAction::triggered, [this, connection]() {
			removeConnection(connection);
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

	for (auto item : this->items()) {
		if (item && item->type() == (int)GraphicsItemType::Node) {
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

void GraphNodeScene::wheelEvent(QGraphicsSceneWheelEvent * event)
{
	QGraphicsScene::wheelEvent(event);
}

void GraphNodeScene::drawItems(QPainter * painter, int numItems, QGraphicsItem * items[], const QStyleOptionGraphicsItem options[], QWidget * widget)
{
	QGraphicsScene::drawItems(painter, numItems, items, options, widget);
}

void GraphNodeScene::dropEvent(QGraphicsSceneDragDropEvent * event)
{
	event->accept();
	auto node = nodeGraph->library->createNode(event->mimeData()->text());

	//	auto factory = nodeGraph->modelFactories[event->mimeData()->text()];
	if (node) {
		this->addNodeModel(node, event->scenePos().x(), event->scenePos().y());
		return;
	}
	qDebug() << event->mimeData()->data("index").toInt();
	auto prop = nodeGraph->properties.at(event->mimeData()->data("index").toInt());
	qDebug() << prop << prop->id << prop->displayName << prop->getValue();
	if (prop) {
		auto propNode = new PropertyNode();
		propNode->setProperty(prop);
		this->addNodeModel(propNode, event->scenePos().x(), event->scenePos().y());
		qDebug() << "no node element";
	}
}

void GraphNodeScene::drawBackground(QPainter * painter, const QRectF & rect)
{
	//does not draw background
	//painter->setBackground(QBrush(QColor(00, 200, 200, 50)));
	//painter->fillRect(rect, QBrush(QColor(00, 00, 0, 200)));
//	QGraphicsScene::drawBackground(painter, rect);
}

GraphNodeScene::GraphNodeScene(QWidget* parent) :
	QGraphicsScene(parent)
{
	nodeGraph = nullptr;
	con = nullptr;
	this->installEventFilter(this);
	conGroup = new QGraphicsItemGroup;
	addItem(conGroup);

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

SocketConnection * GraphNodeScene::removeConnection(SocketConnection * connection)
{
	auto socket1 = connection->socket1;
	auto socket2 = connection->socket2;
	socket1->removeConnection(connection);
	socket2->removeConnection(connection);

	this->removeItem(connection);
	this->nodeGraph->removeConnection(connection->connectionId);

	emit connectionRemoved(connection);
	return connection;
}


bool GraphNodeScene::eventFilter(QObject *o, QEvent *e)
{
	QGraphicsSceneMouseEvent *me = (QGraphicsSceneMouseEvent*)e;

	switch ((int)e->type())
	{
	case QEvent::GraphicsSceneMousePress:
	{
		auto sock = getSocketAt(me->scenePos().x(), me->scenePos().y());
		if (sock != nullptr) {
			if (me->button() == Qt::LeftButton) {

				// if it's an insocket with a connection then we're removing the connection
				if (sock->socketType == SocketType::In && sock->connections.size() == 1) {
					// insockets only have one connection
					con = sock->connections[0];

					// remove connection from nodegraph model
					this->nodeGraph->removeConnection(con->connectionId);

					// should be socket2 of the connection
					con->socket1->removeConnection(con);
					con->socket2->removeConnection(con);

					// emit event before modifying the nodes
					emit connectionRemoved(con);

					con->socket2 = nullptr;
					con->status = SocketConnectionStatus::Started;

					con->pos2 = me->scenePos();
					con->updatePath();
					socketConnections.removeOne(con);

					views().at(0)->setDragMode(QGraphicsView::NoDrag);

				}
				else {
					con = new SocketConnection();
					con->socket1 = sock;
					con->pos1 = me->scenePos();
					con->pos2 = me->scenePos();
					con->status = SocketConnectionStatus::Started;
					con->updatePath();
					//conGroup->addToGroup(con);
					this->addItem(con);
					views().at(0)->setDragMode(QGraphicsView::NoDrag);
				}

			}
			if (me->button() == Qt::RightButton) {
				auto x = me->scenePos().x();
				auto y = me->scenePos().y();
				auto menu = removeConnectionContextMenu(x, y);
				auto view = this->views().first();
				auto scenePoint = view->mapFromScene(me->scenePos());
				auto p = view->viewport()->mapToGlobal(scenePoint);

				menu->exec(p);
			}

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
		else if (me->button() == Qt::MiddleButton)
		{
			views().at(0)->setDragMode(QGraphicsView::NoDrag);
		}
	}
	break;
	case QEvent::GraphicsSceneMouseMove:
	{
		if (con) {
			con->pos2 = me->scenePos();
			con->updatePath();

			auto sock = getSocketAt(me->scenePos().x(), me->scenePos().y());
			if (sock != nullptr && con->socket1 != sock) {
				qDebug() << "connection entered";
			}
			return true;
		}
	}
	break;
	case QEvent::GraphicsSceneMouseRelease:
	{
		views().at(0)->setDragMode(QGraphicsView::RubberBandDrag);


		if (con) {
			// make it an official connection
			auto sock = getSocketAt(me->scenePos().x(), me->scenePos().y());
			if (sock != nullptr) {
				// ensure only outs can connect to ins
				if (sock->socketType != con->socket1->socketType) {
					// todo: prevent recursive connection
					if (con->socket1->node != sock->node) {// prevent it connecting to itself
						con->socket2 = sock;
						con->status = SocketConnectionStatus::Finished;
						con->socket1->addConnection(con);
						con->socket2->addConnection(con);
						con->updatePosFromSockets();
						con->updatePath();
						socketConnections.append(con);

						// connect models too
						if (nodeGraph != nullptr) {
							// check the order of nodes
							if (con->socket1->socketType == SocketType::Out) {
								auto conModel = this->nodeGraph->addConnection(con->socket1->node->nodeId, con->socket1->socketIndex,
									con->socket2->node->nodeId, con->socket2->socketIndex);
								con->connectionId = conModel->id; // very important!
							}
							else {
								auto conModel = this->nodeGraph->addConnection(con->socket2->node->nodeId, con->socket2->socketIndex,
									con->socket1->node->nodeId, con->socket1->socketIndex);
								con->connectionId = conModel->id; // very important!
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

	case QEvent::GraphicsSceneDrop: {
		auto event = (QDropEvent*)e;
		event->acceptProposedAction();
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

Socket* GraphNodeScene::getConnectionAt(float x, float y)
{
	auto items = this->items(QPointF(x, y));
	//auto items = this->items();
	for (auto item : items) {
		if (item && item->type() == (int)GraphicsItemType::Connection)
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

QVector<GraphNode*> GraphNodeScene::getNodes()
{
	QVector<GraphNode*> nodes;
	auto items = this->items();
	for (auto item : items) {
		if (item && item->type() == (int)GraphicsItemType::Node)
				nodes.append((GraphNode*)item);
	}

	return nodes;
}

GraphNode *GraphNodeScene::getNodeByPos(QPointF point)
{
	auto items = this->items();
	//auto items = this->items();
	for (auto item : items) {
		if (item && item->boundingRect().contains(point)) {
			return (GraphNode*)item;
		}
	}

	return nullptr;
}

