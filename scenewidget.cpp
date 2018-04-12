#include "scenewidget.h"
#include <irisgl/IrisGL.h>
#include "irisgl/src/graphics/graphicshelper.h"

QString assetPath(QString relPath)
{
    //qDebug()<< QDir::cleanPath(QCoreApplication::applicationFilePath() + QDir::separator() + relPath);
    //return QDir::cleanPath(QCoreApplication::applicationFilePath() + QDir::separator() + relPath);
    qDebug() << QDir::cleanPath(QDir::currentPath() + QDir::separator() + relPath);
    return QDir::cleanPath(QDir::currentPath() + QDir::separator() + relPath);
}

void SceneWidget::start()
{
    iris::VertexLayout layout;
    layout.addAttrib(iris::VertexAttribUsage::Position, GL_FLOAT, 3, sizeof (float) * 3);
    vertexBuffer = iris::VertexBuffer::create(layout);
    //vertexBuffer->setData()

    cam = iris::CameraNode::create();
    cam->setLocalPos(QVector3D(2, 2, 3));
    cam->lookAt(QVector3D(0,0,0));
    /*
    shader = iris::Shader::load(
                ":assets/shaders/color.vert",
                ":assets/shaders/color.frag");
                */

    mesh = iris::Mesh::loadMesh(assetPath("assets/sphere.obj"));
    //mat = iris::DefaultMaterial::create();

    font = iris::Font::create(device);

    vertString = iris::GraphicsHelper::loadAndProcessShader(assetPath("assets/surface.vert"));
    fragString = iris::GraphicsHelper::loadAndProcessShader(assetPath("assets/surface.frag"));
    updateShader("void surface(inout Material material){}");

    renderTime = 0;

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
}

void SceneWidget::update(float dt)
{
    cam->aspectRatio = width()/(float)height();
    cam->update(dt);

    //qDebug()<<1.0/dt;
    fps = 1.0/dt;
    renderTime += dt;
}

void SceneWidget::render()
{
    device->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, Qt::white);

    spriteBatch->begin();
    spriteBatch->drawString(font, QString("fps %1").arg(fps), QVector2D(), Qt::black);
    spriteBatch->end();

    device->setBlendState(iris::BlendState::createOpaque());
    device->setDepthState(iris::DepthState());

    auto& graphics = device;
    device->setViewport(QRect(0, 0, width(), height()));
    device->setShader(shader);
    device->setShaderUniform("u_viewMatrix", cam->viewMatrix);
    device->setShaderUniform("u_projMatrix", cam->projMatrix);
    QMatrix4x4 world;
    world.setToIdentity();
    device->setShaderUniform("u_worldMatrix", world);
    device->setShaderUniform("u_normalMatrix", world.normalMatrix());
    device->setShaderUniform("color", QVector4D(0.0, 0.0, 0.0, 1.0));
    device->setShaderUniform("u_textureScale", 1.0f);

    graphics->setShaderUniform("u_eyePos", cam->getLocalPos());
    graphics->setShaderUniform("u_sceneAmbient", QVector3D(0,0,0));

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

    mesh->draw(device);

}

void SceneWidget::updateShader(QString shaderCode)
{
    shader = iris::Shader::create(
                vertString,
                fragString + shaderCode);
}

SceneWidget::SceneWidget():
    iris::RenderWidget(nullptr)
{
}
