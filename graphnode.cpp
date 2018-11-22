#include "graphnode.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsEffect>
#include "socket.h"
#include "socketconnection.h"
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QTimer>

#include <irisgl/IrisGL.h>
#include <irisgl/Graphics.h>
#include "graph/nodegraph.h"
#include "graphnodescene.h"
#include "texturemanager.h"
//#include "irisgl/src/graphics/graphicshelper.h"

CustomRenderWidget::CustomRenderWidget() :
	iris::RenderWidget(nullptr)
{

}

QString CustomRenderWidget::assetPath(QString relPath)
{
	//qDebug()<< QDir::cleanPath(QCoreApplication::applicationFilePath() + QDir::separator() + relPath);
	//return QDir::cleanPath(QCoreApplication::applicationFilePath() + QDir::separator() + relPath);
	qDebug() << QDir::cleanPath(QDir::currentPath() + QDir::separator() + relPath);
	return QDir::cleanPath(QDir::currentPath() + QDir::separator() + relPath);
}

void CustomRenderWidget::start()
{
	//targetFPS = 0;

	scale = 1.0;

	iris::VertexLayout layout;
	layout.addAttrib(iris::VertexAttribUsage::Position, GL_FLOAT, 3, sizeof(float) * 3);
	//vertexBuffer->setData()

	cam = iris::CameraNode::create();
	cam->setLocalPos(QVector3D(2, 0, 3));
	cam->lookAt(QVector3D(0, 0, 0));
	cam->nearClip = 1.0;
	cam->farClip = 5.0f;
	/*
	shader = iris::Shader::load(
	":assets/shaders/color.vert",
	":assets/shaders/color.frag");
	*/

	mesh = iris::Mesh::loadMesh(assetPath("assets/lowpoly_sphere.obj"));
	//mat = iris::DefaultMaterial::create();

	font = iris::Font::create(device);

	vertString = iris::GraphicsHelper::loadAndProcessShader(assetPath("assets/preview.vert"));
	fragString = iris::GraphicsHelper::loadAndProcessShader(assetPath("assets/preview.frag"));
	updateShader("void preview(inout PreviewMaterial preview){}");

	renderTime = 0;

	lights.clear();

	// setup lights
	auto main = iris::LightNode::create();
	main->setLightType(iris::LightType::Point);
	main->setLocalPos(QVector3D(-3, 0, 3));
	main->setVisible(true);
	main->color = QColor(255, 255, 255);
	main->intensity = 0.8f;
	lights.append(main);
}

void CustomRenderWidget::update(float dt)
{
	cam->aspectRatio = width() / (float)height();
	cam->update(dt);

	//qDebug()<<1.0/dt;
	fps = 1.0 / dt;
	renderTime += dt;
}

void CustomRenderWidget::render()
{
	cam->aspectRatio = width() / (float)height();
	cam->update(0.016f);

	//device->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, QColor(qMin((int)(renderTime*0.1f * 255), 255), 0, 0));
	device->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, QColor(0, 0, 0, 0));

	device->setBlendState(iris::BlendState::createOpaque());
	device->setDepthState(iris::DepthState());

	auto& graphics = device;
	auto w = width();
	auto h = height();
	device->setViewport(QRect(0, 0, w, h));
	device->setShader(shader);
	device->setShaderUniform("u_viewMatrix", cam->viewMatrix);
	device->setShaderUniform("u_projMatrix", cam->projMatrix);
	QMatrix4x4 world;
	world.setToIdentity();
	world.rotate(rot);
	world.scale(scale);

	device->setShaderUniform("u_worldMatrix", world);
	device->setShaderUniform("u_normalMatrix", world.normalMatrix());
	device->setShaderUniform("color", QVector4D(0.0, 0.0, 0.0, 1.0));
	device->setShaderUniform("u_textureScale", 1.0f);

	graphics->setShaderUniform("u_eyePos", cam->getLocalPos());
	graphics->setShaderUniform("u_sceneAmbient", QVector3D(0, 0, 0));
	graphics->setShaderUniform("u_time", renderTime);

	// pass textures
	auto texMan = TextureManager::getSingleton();
	texMan->loadUnloadedTextures();
	int i = 0;
	for (auto tex : texMan->textures) {
		graphics->setTexture(i, tex->texture);
		graphics->setShaderUniform(tex->uniformName, i);
		i++;
	}

	passNodeGraphUniforms();

	graphics->setShaderUniform("u_lightCount", lights.size());

	mesh->draw(device);
}

void CustomRenderWidget::updateShader(QString shaderCode)
{
	shader = iris::Shader::create(
		vertString,
		fragString + shaderCode);

	qDebug() << "-------- PREVIEW SHADER --------";
	qDebug().noquote() << fragString + shaderCode;
	qDebug() << "-------- PREVIEW SHADER --------";
}

void CustomRenderWidget::resetRenderTime()
{
	renderTime = 0;
}

void CustomRenderWidget::passNodeGraphUniforms()
{
	if (graph == nullptr)
		return;

	for (auto prop : graph->properties) {
		switch (prop->type) {
		case PropertyType::Bool:
			device->setShaderUniform(prop->getUniformName(), prop->getValue().toBool());
			break;
		case PropertyType::Int:
			device->setShaderUniform(prop->getUniformName(), prop->getValue().toInt());
			break;
		case PropertyType::Float:
			//qDebug()<<prop->getUniformName()<<" - "<<prop->getValue().toFloat();
			device->setShaderUniform(prop->getUniformName(), prop->getValue().toFloat());
			break;
		case PropertyType::Vec2:
			device->setShaderUniform(prop->getUniformName(), prop->getValue().value<QVector2D>());
			break;
		case PropertyType::Vec3:
			device->setShaderUniform(prop->getUniformName(), prop->getValue().value<QVector3D>());
			break;
		case PropertyType::Vec4:
			device->setShaderUniform(prop->getUniformName(), prop->getValue().value<QVector4D>());
			break;
		}
	}
}

void CustomRenderWidget::setNodeGraph(NodeGraph *graph)
{
	this->graph = graph;
}

GraphNode::GraphNode(QGraphicsItem* parent) :
	QGraphicsPathItem(parent)
{
	nodeType = 0;
	proxyWidget = nullptr;


	this->setFlag(QGraphicsItem::ItemIsMovable);
	this->setFlag(QGraphicsItem::ItemIsSelectable);
	this->setFlag(QGraphicsItem::ItemSendsGeometryChanges);
	this->setFlag(QGraphicsItem::ItemSendsScenePositionChanges);

	this->setCacheMode(QGraphicsItem::DeviceCoordinateCache);

	nodeWidth = 170;
	QPainterPath path_content;
	// path_content.setFillRule(Qt::WindingFill);
	//path_content.addRoundedRect(QRect(0, 0, 100, 220), 5, 5);
	//path_content.addRect(QRect(0, 0, nodeWidth, calcHeight()));
	//path_content.addRoundedRect(QRect(0, 0, nodeWidth, calcHeight()),17,17);
	setPath(path_content);

	setPen(QPen(Qt::black));
	setBrush(QColor(240, 240, 240));

	text = new QGraphicsTextItem(this);
	text->setPlainText("Title");

	
	text->setPos(5, 16);
	text->setDefaultTextColor(QColor(255, 255, 255));

	QFont font = text->font();
	font.setWeight(65);
	auto ratio = QApplication::desktop()->devicePixelRatio();
	font.setPointSize(8 * ratio);
	text->setFont(font);

	/*
	QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
	effect->setBlurRadius(12);
	effect->setXOffset(0);
	effect->setYOffset(0);
	effect->setColor(QColor(00, 00, 00, 40));
	setGraphicsEffect(effect);
	*/

	// preview widget
	proxyPreviewWidget = nullptr;
	previewWidget = nullptr;
}

void GraphNode::setIcon(QIcon icon)
{
	this->icon = icon;
}

void GraphNode::setTitleColor(QColor color)
{
	titleColor = color;

}

void GraphNode::setTitle(QString title)
{
	text->setPlainText(title);
	auto textWidth = text->boundingRect().width();
	auto textRect = text->boundingRect();
	auto textHeight = textRect.height();
	text->setPos(nodeWidth / 2 - textWidth / 2, titleHeight / 2 - textHeight / 2);
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
	proxyWidget->setPos((nodeWidth - proxyWidget->size().width()) / 2,
		y);

	calcPath();

	layout();
}

void GraphNode::calcPath()
{
	QPainterPath path_content;
	path_content.setFillRule(Qt::WindingFill);
	path_content.addRoundedRect(QRect(0, 0, nodeWidth, calcHeight()), titleRadius, titleRadius);
	setPath(path_content);
}

int GraphNode::calcHeight()
{
	int height = 0;
	height += titleHeight + 20;// title + padding

	for (auto socket : sockets)
	{
		height += socket->calcHeight();
		height += 15; // padding
	}

	//height += 2; // padding
	if (proxyWidget != nullptr)
		height += proxyWidget->size().height();

	return height;
}

Socket *GraphNode::getInSocket(int index)
{
	int i = 0;
	for (auto sock : sockets) {
		if (sock->socketType == SocketType::In) {
			if (index == i)
				return sock;
			i++;
		}
	}

	return nullptr;
}

Socket *GraphNode::getOutSocket(int index)
{
	int i = 0;
	for (auto sock : sockets) {
		if (sock->socketType == SocketType::Out) {
			if (index == i)
				return sock;
			i++;
		}
	}
}

void GraphNode::layout()
{
	int height = 0;
	height += titleHeight + 20;// title + padding

	for (auto socket : sockets)
	{
		height += socket->calcHeight();
		height += 14; // padding
	}

	//height += 2; // padding
	if (proxyWidget != nullptr) {
		proxyWidget->setPos((nodeWidth - proxyWidget->size().width()) / 2,
			height);
		height += proxyWidget->size().height();
	}

	if (proxyPreviewWidget != nullptr) {
		height += 5;
		proxyPreviewWidget->setPos((nodeWidth - proxyPreviewWidget->size().width()) / 2,
			height);
		height += proxyPreviewWidget->size().height();
	}

	height += 5;
		
	// calculate path
	QPainterPath path_content;
	path_content.setFillRule(Qt::WindingFill);
	path_content.addRoundedRect(QRect(0, 0, nodeWidth, height), titleRadius, titleRadius);
	setPath(path_content);
}

void GraphNode::setPreviewShader(QString shader)
{
	if (this->previewWidget != nullptr) {
		this->previewWidget->updateShader(shader);
		this->update();
	}
}

void GraphNode::enablePreviewWidget()
{
	proxyPreviewWidget = new QGraphicsProxyWidget(this);
	previewWidget = new CustomRenderWidget();
	proxyPreviewWidget->setWidget(previewWidget);
	proxyPreviewWidget->setGeometry(QRectF(0, 260, 160, 160));

	if (this->nodeGraph)
		previewWidget->setNodeGraph(nodeGraph);

	auto updateTimer = new QTimer();
	QObject::connect(updateTimer, &QTimer::timeout, [this]()
	{
		//this->update();
		proxyPreviewWidget->update();
	});
	updateTimer->start(1000 / 30);

	layout();
}

void GraphNode::setNodeGraph(NodeGraph* graph)
{
	this->nodeGraph = graph;
	if (this->previewWidget) {
		this->previewWidget->setNodeGraph(graph);
	}
}

void GraphNode::paint(QPainter *painter,
	const QStyleOptionGraphicsItem *option,
	QWidget *widget)
{
	//painter->setRenderHint(QPainter::HighQualityAntialiasing);
	//painter->setRenderHint(QPainter::Antialiasing);
	//painter->setRenderHint(QPainter::TextAntialiasing);
	//painter->setRenderHint(QPainter::SmoothPixmapTransform);

	if (option->state.testFlag(QStyle::State_Selected) != currentSelectedState) {
		currentSelectedState = option->state.testFlag(QStyle::State_Selected);
		highlightNode(currentSelectedState, 0);
	}

	if (isHighlighted && level == 0) {
		auto rect = boundingRect();
		painter->setPen(QPen(connectedColor, 3));
		painter->drawRoundedRect(rect, titleRadius, titleRadius);
	}
	else if (isHighlighted && level > 0) {
		auto rect = boundingRect();
		painter->setPen(QPen(QColor(250, 250, 50), 3));
		painter->drawRoundedRect(rect, titleRadius, titleRadius);
	}



	





	painter->setPen(pen());
	setBrush(QColor(45, 45, 51));
	painter->fillPath(path(), brush());

	// title tab
	QPainterPath titlePath;
	titlePath.setFillRule(Qt::WindingFill);
	//titlePath.addRoundedRect(0, 0, nodeWidth, 35, 7, 7);
	titlePath.addRect(0, 10, nodeWidth, titleHeight-10);
	titlePath.addRoundedRect(0, 0, nodeWidth, titleHeight, titleRadius, titleRadius);
	painter->fillPath(titlePath, QBrush(titleColor));

	//draw icon
	//painter->drawPixmap(5, 5, 15, 15, icon.pixmap(50, 50));
	
	

	//draw text node seperator
	QPainterPath block;
	block.setFillRule(Qt::WindingFill);
	block.addRect(0, titleHeight , nodeWidth, 3);
	painter->fillPath(block, QBrush(QColor(30, 30, 30, 160)));

	QPen pen(QColor(00, 00, 00, 250), .5);
	painter->setPen(pen);
//	painter->drawRoundedRect(boundingRect(), titleRadius, titleRadius);

	auto rect = boundingRect();
	painter->setPen(QPen(titleColor, 3));
	painter->drawRoundedRect(rect, titleRadius, titleRadius);

}

int GraphNode::type() const
{
	return (int)GraphicsItemType::Node;
}

void GraphNode::highlightNode(bool val, int lvl)
{
	isHighlighted = val;
	level = lvl;

	auto setLevel = [val, lvl]() {
		int i = 0;
		if (lvl == 0)  i = 0;
		else  i = lvl + 1;
		return i;

	};

	for (Socket* sock : sockets) {
		if (sock->socketType == SocketType::In) {
			for (SocketConnection* con : sock->connections) {
				if (con->socket1->socketType == SocketType::Out) {
					con->socket1->owner->isHighlighted = val;
					con->socket1->owner->highlightNode(val, level + 1);
					con->socket1->owner->currentSelectedState = false;
				}
				if (con->socket2->socketType == SocketType::Out) {
					con->socket2->owner->isHighlighted = val;
					con->socket2->owner->highlightNode(val, level + 1);
					con->socket2->owner->currentSelectedState = false;

				}
			}
		}
	}
	if (!val) check = val;
	update();
}

void GraphNode::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	check = true;
	QGraphicsPathItem::mousePressEvent(event);

}
