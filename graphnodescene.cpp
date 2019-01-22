#include "graphnodescene.h"
#include "nodes/test.h"
#include "core/project.h"

#include <QMimeData>
#include <QListWidgetItem>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QShortcut>

#include <shadergraph/dialogs/searchdialog.h>




NodeGraph *GraphNodeScene::getNodeGraph() const
{
	return nodeGraph;
}

GraphNode * GraphNodeScene::getNodeByPropertyId(QString id)
{
	return nullptr;
}

void GraphNodeScene::refreshNodeTitle(QString id)
{
	auto node = getNodeById(id);
	if (node) {
		
	}
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
		this->addNodeModel(node, false);
	}

	// add connections
	for (auto con : graph->connections.values()) {
		auto leftNode = con->leftSocket->node;
		auto rightNode = con->rightSocket->node;
		auto conn = this->addConnection(leftNode->id,
			leftNode->outSockets.indexOf(con->leftSocket),
			rightNode->id,
			rightNode->inSockets.indexOf(con->rightSocket));

		conn->connectionId = con->id;// very important!
	}
}

void GraphNodeScene::addNodeModel(NodeModel* model, bool addToGraph)
{
	addNodeModel(model, model->getX(), model->getY(), addToGraph);
}

// add
void GraphNodeScene::addNodeModel(NodeModel *model, float x, float y, bool addToGraph)
{
	auto nodeView = this->createNode<GraphNode>();
	nodeView->setNodeGraph(this->nodeGraph);
	nodeView->setModel(model);
	nodeView->setTitle(model->title);
	nodeView->setTitleColor(model->setNodeTitleColor());
//	if (model->title == "Color Node") nodeView->doNotCheckProxyWidgetHeight = true;

	//nodeView->setIcon(model->icon);

	for (auto sock : model->inSockets)
		nodeView->addInSocket(sock);
	for (auto sock : model->outSockets)
		nodeView->addOutSocket(sock);

	if (model->widget != nullptr) {
		nodeView->setWidget(model->widget);
	}

	
	/*nodeView->setTitle(model->title);
	nodeView->setTitleColor(model->setNodeTitleColor());*/

	nodeView->setPos(model->getX(), model->getY());
	nodeView->nodeId = model->id;
	nodeView->layout();
//	if (model->title == "Color Node") nodeView->resetPositionForColorWidget();

	if (model->isPreviewEnabled()) {
		nodeView->enablePreviewWidget();
	}

	if (addToGraph) {
		Q_ASSERT_X(nodeGraph != nullptr, "GraphNodeScene::addNodeModel", "Cant add node to null scene");
		nodeGraph->addNode(model);
	}

	connect(model, &NodeModel::valueChanged, [this](NodeModel* nodeModel, int sockedIndex) {
		emit nodeValueChanged(nodeModel, sockedIndex);
		emit graphInvalidated();
	});

	auto addNodeCommand = new AddNodeCommand(nodeView, this);
	stack->push(addNodeCommand);
}

QMenu *GraphNodeScene::createContextMenu(float x, float y)
{
	auto menu = new QMenu();
    menu->setStyleSheet(
        "QMenu { background-color: #1A1A1A; color: #EEE; padding: 0; margin: 0; }"
        "QMenu::item { background-color: #1A1A1A; padding: 6px 8px; margin: 0; }"
        "QMenu::item:selected { background-color: #3498db; color: #EEE; padding: 6px 8px; margin: 0; }"
        "QMenu::item : disabled { color: #555; }"
    );

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
			node->setX(x);
			node->setY(y);
			this->addNodeModel(node);

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
    menu->setStyleSheet(
        "QMenu { background-color: #1A1A1A; color: #EEE; padding: 0; margin: 0; }"
        "QMenu::item { background-color: #1A1A1A; padding: 6px 8px; margin: 0; }"
        "QMenu::item:selected { background-color: #3498db; color: #EEE; padding: 6px 8px; margin: 0; }"
        "QMenu::item : disabled { color: #555; }"
    );
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

void GraphNodeScene::updatePropertyNodeTitle(QString title, QString propId)
{

	auto propList = nodeGraph->getNodesByTypeName("property");

	for (auto node : propList) {
		auto propNode = static_cast<PropertyNode *>(node);
		if (propNode->getProperty()->id == propId) {
			auto node = getNodeById(propNode->id);
			node->setTitle(title);
		}
	}

	
}

void GraphNodeScene::addNodeFromSearchDialog(QListWidgetItem * item, const QPoint &point)
{
	auto view = this->views().first();
	auto viewPoint = view->viewport()->mapFromGlobal(point);
	auto scenePoint = view->mapToScene(viewPoint);

	auto p = scenePoint;


	if (!item->data(MODEL_EXT_ROLE).isNull()) {
		auto prop = nodeGraph->properties.at(item->data(MODEL_EXT_ROLE).toInt());
		if (prop) {
			auto propNode = new PropertyNode();
			propNode->setProperty(prop);
			propNode->setX(p.x());
			propNode->setY(p.y());
			this->addNodeModel(propNode);
		}
	}

	if (item->data(MODEL_TYPE_ROLE).toString() == "node") {
		auto node = nodeGraph->library->createNode(item->data(Qt::UserRole).toString());

		//	auto factory = nodeGraph->modelFactories[event->mimeData()->text()];
		if (node) {
			this->addNodeModel(node, p.x(), p.y());
			return;
		}
	}
}

void GraphNodeScene::deleteSelectedNodes()
{
	auto items = selectedItems();
	QList<GraphNode*> nodes;
	auto masterNodeId = nodeGraph->getMasterNode()->id;

	// gather all nodes
	for (auto item : items) {
		if (item->type() == (int)GraphicsItemType::Node) {
			auto node = static_cast<GraphNode*>(item);
			if (node->nodeId != masterNodeId)
				nodes.append(node);
		}
	}

	// remove each node's connections then remove the nodes
	for (auto node : nodes) {
		deleteNode(node);
	}

	emit graphInvalidated();
}

void GraphNodeScene::deleteNode(GraphNode* node)
{
	// remove in and out connections
	auto conns = nodeGraph->getNodeConnections(node->nodeId);

	// remove node from scenegraph
	nodeGraph->removeNode(node->nodeId);

	// remove them from scene
	for (auto con : conns) {
		// false, because the connection has already been removed
		// in nodeGraph->removeNode(node->nodeId);
		this->removeConnection(con->id, false, true);
	}

	this->removeItem(node);

	emit nodeRemoved(node);
}

bool GraphNodeScene::areSocketsComptible(Socket* outSock, Socket* inSock)
{
	// get the socket models and check if they're compatible
	auto outSockModel = nodeGraph->getNode(outSock->node->nodeId)->outSockets[outSock->socketIndex];
	auto inSockModel = nodeGraph->getNode(inSock->node->nodeId)->inSockets[inSock->socketIndex];

	return outSockModel->canConvertTo(inSockModel);
}

void GraphNodeScene::dropEvent(QGraphicsSceneDragDropEvent * event)
{

	if (!event->mimeData()->data("index").isNull()) {
		event->accept();
		auto prop = nodeGraph->properties.at(event->mimeData()->data("index").toInt());
		if (prop) {
			auto propNode = new PropertyNode();
			propNode->setProperty(prop);
			propNode->setX(event->scenePos().x());
			propNode->setY(event->scenePos().y());
			this->addNodeModel(propNode);
		}
	}

	if (0 == QString("node").compare(QString(event->mimeData()->data("MODEL_TYPE_ROLE")))) {
		event->accept();

		auto node = nodeGraph->library->createNode(event->mimeData()->html());

			if (node) {
				this->addNodeModel(node, event->scenePos().x(), event->scenePos().y());
					return;
			}
	}

	if (QVariant(event->mimeData()->data("MODEL_TYPE_ROLE")).toInt() == static_cast<int>(ModelTypes::Shader)) {
		event->accept();

		QListWidgetItem *item = new QListWidgetItem;

		item->setData(Qt::DisplayRole, event->mimeData()->text());
		item->setData(MODEL_GUID_ROLE, event->mimeData()->data("MODEL_GUID_ROLE"));
		currentlyEditing = item;

		emit loadGraph(item);
		return;
	}
}

void GraphNodeScene::drawBackground(QPainter * painter, const QRectF & rect)
{
	//does not draw background
}

GraphNodeScene::GraphNodeScene(QWidget* parent) :
	QGraphicsScene(parent)
{
	nodeGraph = nullptr;
	con = nullptr;
	this->installEventFilter(this);
	conGroup = new QGraphicsItemGroup;
	addItem(conGroup);
	stack = new QUndoStack;
	
	selectedNode = nullptr;
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

SocketConnection * GraphNodeScene::removeConnection(SocketConnection * connection, bool removeFromNodeGraph, bool emitSignal)
{
	auto socket1 = connection->socket1;
	auto socket2 = connection->socket2;
	socket1->removeConnection(connection);
	socket2->removeConnection(connection);
	this->removeItem(connection);

	if (removeFromNodeGraph)
		this->nodeGraph->removeConnection(connection->connectionId);

	if (emitSignal)
		emit connectionRemoved(connection);

	return connection;
}

void GraphNodeScene::removeConnection(const QString& conId, bool removeFromNodeGraph, bool emitSignal)
{
	auto con = getConnection(conId);
	removeConnection(con, removeFromNodeGraph, emitSignal);
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
					//socketConnections.removeOne(con);

					views().at(0)->setDragMode(QGraphicsView::NoDrag);

					emit graphInvalidated();

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
			auto view = this->views().first();
			auto scenePoint = view->mapFromScene(me->scenePos());
			auto p = view->viewport()->mapToGlobal(scenePoint);

//			menu->exec(p);

            auto dialog = new SearchDialog(this->nodeGraph, this, p);
            dialog->exec();
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
						if (areSocketsComptible(con->socket1, sock)) {

							con->socket2 = sock;
							con->status = SocketConnectionStatus::Finished;
							con->socket1->addConnection(con);
							con->socket2->addConnection(con);
							con->updatePosFromSockets();
							con->updatePath();

							

							// connect models too
							if (nodeGraph != nullptr) {
								// check the order of nodes
								AddConnectionCommand *addConnectionCommand;
								if (con->socket1->socketType == SocketType::Out) {
									auto conModel = this->nodeGraph->addConnection(con->socket1->node->nodeId, con->socket1->socketIndex,
										con->socket2->node->nodeId, con->socket2->socketIndex);
									con->connectionId = conModel->id; // very important!
									//push connections to undo redo stack
									addConnectionCommand = new AddConnectionCommand(con, this, con->socket1->socketIndex, con->socket2->socketIndex);
								}
								else {
									auto conModel = this->nodeGraph->addConnection(con->socket2->node->nodeId, con->socket2->socketIndex,
										con->socket1->node->nodeId, con->socket1->socketIndex);
									con->connectionId = conModel->id; // very important!
									addConnectionCommand = new AddConnectionCommand(con, this, con->socket1->socketIndex, con->socket2->socketIndex);

								}
								stack->push(addConnectionCommand);
							}

							emit newConnection(con);
							con = nullptr;
							emit graphInvalidated();

							return true;
						}
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

SocketConnection* GraphNodeScene::getConnectionAt(float x, float y)
{
	auto items = this->items(QPointF(x, y));
	//auto items = this->items();
	for (auto item : items) {
		if (item && item->type() == (int)GraphicsItemType::Connection)
			return (SocketConnection*)item;
	}

	return nullptr;
}

SocketConnection* GraphNodeScene::getConnection(const QString& conId)
{
	auto items = this->items();
	for (auto item : items) {
		if (item && item->type() == (int)GraphicsItemType::Connection)
			if (((SocketConnection*)item)->connectionId == conId)
				return (SocketConnection*)item;
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