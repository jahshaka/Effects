#include "scenewidget.h"
#include <irisgl/IrisGL.h>
//#include "irisgl/src/graphics/graphicshelper.h"
//#include "nodemodel.h"
#include "properties.h"
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QMouseEvent>
#include <QWheelEvent>
#include <qmath.h>
#include <QOpenGLFunctions>
#include <QImage>

#include "graph/nodegraph.h"
#include "graphnodescene.h"
#include "texturemanager.h"
#include "materialsettingswidget.h"
#include "core/materialhelper.h"

QString assetPath(QString relPath)
{
    //qDebug()<< QDir::cleanPath(QCoreApplication::applicationFilePath() + QDir::separator() + relPath);
    //return QDir::cleanPath(QCoreApplication::applicationFilePath() + QDir::separator() + relPath);
    qDebug() << QDir::cleanPath(QDir::currentPath() + QDir::separator() + relPath);
    return QDir::cleanPath(QDir::currentPath() + QDir::separator() + relPath);
}

// NOTE! Context resets when widget is undocked
void SceneWidget::start()
{
	if (initialized)
		return;

	screenshotRT = iris::RenderTarget::create(500, 500);
	screenshotTex = iris::Texture2D::create(500, 500);
	screenshotRT->addTexture(screenshotTex);

    iris::VertexLayout layout;
    layout.addAttrib(iris::VertexAttribUsage::Position, GL_FLOAT, 3, sizeof (float) * 3);
    vertexBuffer = iris::VertexBuffer::create(layout);
    //vertexBuffer->setData()

	clearColor = Qt::white;

    cam = iris::CameraNode::create();
    cam->setLocalPos(QVector3D(2, 0, 3));
    cam->lookAt(QVector3D(0,0,0));
    /*
    shader = iris::Shader::load(
                ":assets/shaders/color.vert",
                ":assets/shaders/color.frag");
                */

    mesh = iris::Mesh::loadMesh(MaterialHelper::assetPath("lowpoly_sphere.obj"));
    //mat = iris::DefaultMaterial::create();

    font = iris::Font::create(device);

    vertString = iris::GraphicsHelper::loadAndProcessShader(MaterialHelper::assetPath("surface.vert"));
    fragString = iris::GraphicsHelper::loadAndProcessShader(MaterialHelper::assetPath("surface.frag"));
	generatedVertString = "void surface(inout vec3 vertexOffset){}";
	generatedFragString = "void surface(inout Material material){}";
	//updateShader("void surface(inout Material material){}");
	updateShader();

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

    auto light = iris::LightNode::create();
    light->setLightType(iris::LightType::Directional);
    light->setLocalPos(QVector3D(0, 3, 0));
    light->setVisible(true);
    light->color = QColor(255, 255, 255);
    light->intensity = 1;
    lights.append(light);

    light = iris::LightNode::create();
    light->setLightType(iris::LightType::Point);
    light->setLocalPos(QVector3D(3, 0, 3));
    light->setVisible(true);
    light->color = QColor(255, 255, 255);
    light->intensity = 1;
    lights.append(light);

	initialized = true;
}

void SceneWidget::update(float dt)
{
    cam->aspectRatio = viewportWidth/(float)viewportHeight;
    cam->update(dt);

    //qDebug()<<1.0/dt;
    fps = 1.0/dt;
    renderTime += dt;
}

void SceneWidget::render()
{
    device->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, clearColor);
	
    //spriteBatch->begin();
    //spriteBatch->drawString(font, QString("fps %1").arg(fps), QVector2D(), Qt::black);
    //spriteBatch->end();
	
    device->setBlendState(iris::BlendState::createOpaque(), true);
    device->setDepthState(iris::DepthState(), true);
	device->setRasterizerState(iris::RasterizerState::createCullCounterClockwise(), true);

    auto& graphics = device;
    device->setViewport(QRect(0, 0, viewportWidth, viewportHeight));
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
    graphics->setShaderUniform("u_sceneAmbient", QVector3D(0,0,0));
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
	

    // lights
    //qDebug()<<lights.size();
    graphics->setShaderUniform("u_lightCount", lights.size());

    for (int i=0;i<lights.size();i++)
    {
        QString lightPrefix = QString("u_lights[%0].").arg(i);

        auto light = lights[i];
        if(!light->isVisible())
        {
            //quick hack for now
            graphics->setShaderUniform(lightPrefix+"color", QColor(0,0,0));
            continue;
        }

        graphics->setShaderUniform(lightPrefix+"type", (int)light->lightType);
        graphics->setShaderUniform(lightPrefix+"position", light->getLocalPos());
        //mat->setUniformValue(lightPrefix+"direction", light->getDirection());
        graphics->setShaderUniform(lightPrefix+"distance", light->distance);
        graphics->setShaderUniform(lightPrefix+"direction", light->getLightDir());
        graphics->setShaderUniform(lightPrefix+"cutOffAngle", light->spotCutOff);
        graphics->setShaderUniform(lightPrefix+"cutOffSoftness", light->spotCutOffSoftness);
        graphics->setShaderUniform(lightPrefix+"intensity", light->intensity);
        graphics->setShaderUniform(lightPrefix+"color", light->color);

        graphics->setShaderUniform(lightPrefix+"constantAtten", 1.0f);
        graphics->setShaderUniform(lightPrefix+"linearAtten", 0.0f);
        graphics->setShaderUniform(lightPrefix+"quadtraticAtten", 1.0f);

        graphics->setShaderUniform(lightPrefix + "shadowType", (int)iris::ShadowMapType::None);
    }


    passNodeGraphUniforms();

    mesh->draw(device);

	//device->setRasterizerState(iris::RasterizerState::createCullNone(), true);
	//device->setDepthState(iris::DepthState(false, false), true);
}

void SceneWidget::setVertexShader(QString vertShader)
{
	generatedVertString = vertShader;
}

void SceneWidget::setFragmentShader(QString fragShader)
{
	generatedFragString = fragShader;
}

/*
void SceneWidget::updateShader(QString shaderCode)
{
    shader = iris::Shader::create(
                vertString,
                fragString + shaderCode);
}
*/

void SceneWidget::updateShader()
{
	shader = iris::Shader::create(
		vertString + generatedVertString,
		fragString + generatedFragString);
}

void SceneWidget::resetRenderTime()
{
    renderTime = 0;
}

void SceneWidget::passNodeGraphUniforms()
{
    for( auto prop : graph->properties) {
        switch(prop->type) {
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

void SceneWidget::setNodeGraph(NodeGraph *graph)
{
    this->graph = graph;
}

void SceneWidget::mousePressEvent(QMouseEvent * evt)
{
	dragging = true;
	lastMousePos = evt->pos();
}

void SceneWidget::mouseMoveEvent(QMouseEvent * evt)
{
	if (dragging) {
		auto curPos = evt->pos();
		auto diff = lastMousePos - curPos;
		lastMousePos = curPos;
		float dragScale = 0.7f;

		auto tx = QQuaternion::fromEulerAngles(-diff.y() * dragScale, -diff.x() * dragScale, 0);
		rot = tx * rot;
	}
}

void SceneWidget::mouseReleaseEvent(QMouseEvent * evt)
{
	dragging = false;
}

void SceneWidget::wheelEvent(QWheelEvent* evt)
{
	scale += (evt->delta()/480.0f);
}

QImage SceneWidget::takeScreenshot(int width, int height)
{
	this->makeCurrent();
	auto tempW = viewportWidth;
	auto tempH = viewportHeight;
	viewportWidth = width;
	viewportHeight = height;
	clearColor = QColor(0, 0, 0, 0);

	cam->aspectRatio = viewportWidth / (float)viewportHeight;
	cam->update(0.01f);// just a non-zero value is needed here

	screenshotRT->resize(width, height, true);
	device->setRenderTarget(screenshotRT);
	render();
	device->clearRenderTarget();
	
	viewportWidth = tempW;
	viewportHeight = tempH;
	clearColor = Qt::white;

	auto img = screenshotRT->toImage();
	this->doneCurrent();

	return img;
}

void SceneWidget::resizeEvent(QResizeEvent* evt)
{
	viewportWidth = (int)(width() * devicePixelRatioF());
	viewportHeight = (int)(height() * devicePixelRatioF());

	iris::RenderWidget::resizeEvent(evt);
}

SceneWidget::SceneWidget():
    iris::RenderWidget(nullptr)
{
	rot = QQuaternion::fromEulerAngles(0, 0, 0);
	scale = 1;
	dragging = false;

	generatedVertString = "";
	generatedFragString = "";
}

void SceneWidget::setMaterialSettings(MaterialSettings settings)
{
	this->materialSettings = settings;

	if (settings.blendMode == BlendMode::Additive)
		blendState = iris::BlendState::createAdditive();
	if (settings.blendMode == BlendMode::Blend)
		blendState = iris::BlendState::createAlphaBlend();
	if (settings.blendMode == BlendMode::Opaque)
		blendState = iris::BlendState::createOpaque();
}
