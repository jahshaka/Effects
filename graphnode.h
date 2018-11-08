#pragma once

#include <QGraphicsPathItem>
#include <QGraphicsProxyWidget>
#include <QIcon>
#include <QWindow>
#include <irisgl/IrisGL.h>

class NodeGraph;

class CustomRenderWidget : public iris::RenderWidget
{
	iris::MeshPtr mesh;
	iris::ShaderPtr shader;
	iris::MaterialPtr mat;

	iris::FontPtr font;
	float fps;

	iris::VertexBufferPtr vertexBuffer;
	iris::Texture2DPtr tex;

	iris::CameraNodePtr cam;

	QString vertString;
	QString fragString;

	float renderTime;
	QList<iris::LightNodePtr> lights;

	bool dragging;
	QPoint lastMousePos;
	QQuaternion rot;
	float scale;

	NodeGraph* graph = nullptr;
public:
	CustomRenderWidget();

	void start();

	void update(float dt);

	void render();

	void updateShader(QString shaderCode);

	void resetRenderTime();

	void passNodeGraphUniforms();
	void setNodeGraph(NodeGraph* graph);

	QString assetPath(QString relPath);
};

enum class GraphicsItemType : int
{
	Node = QGraphicsItem::UserType + 1,
	Socket = QGraphicsItem::UserType + 2,
	Connection = QGraphicsItem::UserType + 3
};

class Socket;
class SocketConnection;
class GraphNode : public QGraphicsPathItem
{
	QVector<Socket*> sockets;
	int nodeWidth;
	QGraphicsTextItem* text;
	QGraphicsProxyWidget* proxyWidget;
	QGraphicsProxyWidget* proxyPreviewWidget;
	

	int inSocketCount = 0;
	int outSocketCount = 0;
public:
	int nodeType;
	int level = 0;
	int titleHeight = 25;
	int titleRadius = 2;
	bool isHighlighted = false;
	bool currentSelectedState = false;
	bool check = false;
	QString nodeId;
	QColor titleColor;
	QIcon icon;
	CustomRenderWidget* previewWidget;

	GraphNode(QGraphicsItem* parent);

	void setIcon(QIcon icon);
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

	void layout();

	virtual void paint(QPainter *painter,
		const QStyleOptionGraphicsItem *option,
		QWidget *widget = 0) override;

	virtual int type() const override;
private:
	QColor connectedColor = QColor(50, 150, 250);
	QColor disconnectedColor = QColor(90, 90, 90, 0);
	void highlightNode(bool val, int lvl);
	QFont font;
protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event)override;
};

