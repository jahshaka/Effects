#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "nodegraph.h"
#include "nodemodel.h"
#include <QMouseEvent>
#include <QDebug>
#include "scenewidget.h"
#include <QLayout>
#include <QGridLayout>
#include <QLineEdit>
#include "graphtest.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new GraphNodeScene(this);
    scene->setBackgroundBrush(QBrush(QColor(60, 60, 60)));
    ui->graphicsView->setScene(scene);

    // set default
    /*
    auto masterNode = new NodeModel();
    masterNode->typeName = "Material";
    masterNode->inSockets.append(new Vector3SocketModel("diffuse"));
    masterNode->inSockets.append(new Vector3SocketModel("specular"));
    masterNode->inSockets.append(new FloatSocketModel("shininess"));
    masterNode->inSockets.append(new Vector3SocketModel("normal", "v_normal"));
    masterNode->inSockets.append(new Vector3SocketModel("ambient"));
    masterNode->inSockets.append(new Vector3SocketModel("emission"));
    masterNode->inSockets.append(new FloatSocketModel("alpha"));
    */

    auto masterNode = new SurfaceMasterNode();

    auto graph = new NodeGraph;
    graph->addNode(masterNode);
    graph->setMasterNode(masterNode);
    scene->setNodeGraph(graph);

    registerModels(graph);

    connect(scene, &GraphNodeScene::newConnection, [this](SocketConnection* connection)
    {

        auto graph = scene->getNodeGraph();
        ShaderGenerator shaderGen;
        auto code = shaderGen.generateShader(graph);
        ui->textEdit->setPlainText(code);
        sceneWidget->updateShader(code);
        sceneWidget->resetRenderTime();

    });

    connect(scene, &GraphNodeScene::nodeValueChanged, [this](NodeModel* model, int index)
    {
        auto graph = scene->getNodeGraph();
        ShaderGenerator shaderGen;
        auto code = shaderGen.generateShader(graph);
        ui->textEdit->setPlainText(code);
        sceneWidget->updateShader(code);
        sceneWidget->resetRenderTime();
    });


    // preview widget
    sceneWidget = new SceneWidget();
    auto grid = new QGridLayout();
    grid->setSpacing(0);
    grid->setMargin(0);
    grid->addWidget(sceneWidget);
    ui->sceneContainer->setLayout(grid);
}

MainWindow::~MainWindow()
{
    delete ui;
}
