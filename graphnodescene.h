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
#include <QListWidgetItem>
#include <QMenu>
#include <QPointF>
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
	GraphNode* selectedNode;
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
	SocketConnection* removeConnection(SocketConnection* connection, bool removeFromNodeGraph = true, bool emitSignal = true);
	void removeConnection(const QString& conId, bool removeFromNodeGraph = true, bool emitSignal = true);
	SocketConnection* addConnection(Socket* leftCon, Socket* rightCon);


	bool eventFilter(QObject *o, QEvent *e);
	Socket* getSocketAt(float x, float y);
	SocketConnection* getConnectionAt(float x, float y);
	SocketConnection* getConnection(const QString& conId);

	GraphNode* getNodeById(QString id);
    QVector<GraphNode*> getNodes();
	GraphNode* getNodeByPos(QPointF point);
	//QVector<SocketConnection*> socketConnections;
	NodeGraph *getNodeGraph() const;
	GraphNode* getNodeByPropertyId(QString id);
	void refreshNodeTitle(QString id);
	void setNodeGraph(NodeGraph* value);
	void addNodeModel(NodeModel* model, bool addToGraph = true);
	void addNodeModel(NodeModel* model, float x, float y, bool addToGraph = true);
	void addPropertyNode(Property* prop, float x, float y, bool addToGraph = true);

	QMenu* createContextMenu(float x, float y);
	QMenu* removeConnectionContextMenu(float x, float y);

	QJsonObject serialize();
	QListWidgetItem *currentlyEditing = Q_NULLPTR;
	QList<QString> loadedShadersGUID;

	void setList(QList<QString> list) { loadedShadersGUID = list; }
	void updatePropertyNodeTitle(QString title, QString propId);

	void addNodeFromSearchDialog(QListWidgetItem* item, const QPoint& point);

	void deleteSelectedNodes();
	void deleteNode(GraphNode* node);

	bool areSocketsComptible(Socket* outSock, Socket* inSock);

protected:
	void dropEvent(QGraphicsSceneDragDropEvent *event) override;
	void drawBackground(QPainter *painter, const QRectF &rect);

signals:
	void newConnection(SocketConnection* connection);
	// not emitted when a node is deleted
	void connectionRemoved(SocketConnection* connection);
	void nodeRemoved(GraphNode* connection);
	void nodeValueChanged(NodeModel* nodeModel, int socketIndex);
	void loadGraph(QListWidgetItem *item);

	// called whenever something is done that should cause the shader
	// to be invalidated such as:
	// adding a connection
	// removing a connection
	// deleting a node
	// changing a value
	void graphInvalidated();
};





