#pragma once

#include <QDropEvent>
#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsPathItem>
#include <QGraphicsTextItem>
#include <QGraphicsItem>
#include <QGraphicsObject>
#include <QFontMetrics>
#include <QtMath>
#include <QGraphicsSceneMouseEvent>
#include <QJsonObject>
#include <QJsonArray>
#include <QMenu>
#include <QPointf>
#include <QPropertyAnimation>
#include "socketconnection.h"
#include "graphnode.h"
#include "graph/nodegraph.h"
#include "socket.h"
#include "graph/sockethelper.h"
#include "graph/nodemodel.h"
#include "graph/connectionmodel.h"
#include "graph/nodegraph.h"
#include "graph/library.h"
#include "graph/graphicsview.h"
//#include "nodes/test.h"

#include <QDebug>


class NodeGraph;
class NodeModel;
class Property;
class GraphNodeScene : public QGraphicsScene
{
	Q_OBJECT

		//QVector<GraphNode> nodes;

		// only used when dragging
		SocketConnection* con;

	

	QGraphicsItemGroup *conGroup;
public:
	GraphNodeScene(QWidget* parent);
	// model for scene
	NodeGraph* nodeGraph;
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
	SocketConnection* removeConnection(SocketConnection* connection);
	SocketConnection* addConnection(Socket* leftCon, Socket* rightCon);


	bool eventFilter(QObject *o, QEvent *e);
	Socket* getSocketAt(float x, float y);
	Socket* getConnectionAt(float x, float y);
	GraphNode* getNodeById(QString id);
	QVector<GraphNode*> GraphNodeScene::getNodes();
	GraphNode* getNodeByPos(QPointF point);
	QVector<SocketConnection*> socketConnections;
	NodeGraph *getNodeGraph() const;
	void setNodeGraph(NodeGraph* value);
	void addNodeModel(NodeModel* model, float x, float y, bool addToGraph = true);
	void addPropertyNode(Property* prop, float x, float y, bool addToGraph = true);

	QMenu* createContextMenu(float x, float y);
	QMenu* removeConnectionContextMenu(float x, float y);

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





