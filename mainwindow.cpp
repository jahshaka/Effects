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

struct TempVar
{
    QString name;
    QString typeName;
};

class ShaderContext
{
    // mapped using the socket guid
    QMap<QString, TempVar> tempVars;
public:
    void addFunction(QString name, QString function);

    // created a temporary variable using the socket
    TempVar createTempVar(SocketModel* sock)
    {
        if (tempVars.contains(sock->id))
            return tempVars[sock->id];

        TempVar tempVar;
        tempVar.name = QString("temp%1").arg(tempVars.count());
        tempVar.typeName = sock->typeName;
        tempVars.insert(sock->id, tempVar);

        return tempVar;
    }

    QList<TempVar> getTempVars()
    {
        return tempVars.values();
    }
};

class FloatNodeModel : public NodeModel
{
    QLineEdit* lineEdit;
public:
    FloatNodeModel():
        NodeModel()
    {
        lineEdit = new QLineEdit();
        lineEdit->setValidator(new QDoubleValidator());
        lineEdit->setMaximumSize(lineEdit->sizeHint());

        connect(lineEdit, &QLineEdit::textChanged,
                  this, &FloatNodeModel::editTextChanged);

        lineEdit->setText("0.0");

        this->widget = lineEdit;

        typeName = "float";
        title = "Float";
    }

    void editTextChanged(const QString& text)
    {
        emit valueChanged(this, 0);
    }

    QString getSocketValue(int socketIndex, ModelContext *context)
    {
        return lineEdit->text();
    }

    NodeModel* createDuplicate() override
    {
        return new FloatNodeModel();
    }
};

class ShaderGen
{
public:
    QString evaluateShader(NodeGraph* graph, NodeModel* masterNode)
    {
        ShaderContext ctx;

        // master node is expected to be shader node
        QString code;
        for(int i = 0; i < masterNode->inSockets.count(); i++){
            auto c = evalSocket(ctx, graph, masterNode, i);
            code += c;

            // assign values right away
            auto sock = masterNode->inSockets[i];
            auto tempName = ctx.createTempVar(sock).name;
            code += "material." + sock->name + " = " + tempName+";\n";
        }

        // prepend code with temp variables created
        for(auto& var : ctx.getTempVars()) {
            auto c = var.typeName + " " + var.name + ";\n";
            code = c + code;
        }

        code = "void surface(inout Material material){\n" + code;
        code += "}\n";
        return code;
        // assign values
        //"material.diffuse = " + ctx
    }

    // This is executed in the perspective of a connection
    QString evalSocket(ShaderContext& ctx, NodeGraph* graph, NodeModel* socketNode, int socketIndex)
    {
        QString c = "";
        auto inSock = socketNode->inSockets[socketIndex];
        //TempVar var;
        //NodeModel* leftNode;
        auto var = ctx.createTempVar(inSock);

        // get the connection so we can get the value
        auto con = graph->getConnectionFromOutputNode(socketNode,socketIndex);

        // if there is no connection then get the default value
        if (!con) {
            //auto var = ctx.createTempVar(inSock);
            return var.name + " = " + inSock->getDefaultValue() + ";\n";
        }

        // get node left of connection and create temp var
        auto leftNode = graph->nodes[con->leftNodeId];
        auto outSock = leftNode->outSockets[con->leftSocketIndex];
        auto leftVar = ctx.createTempVar(outSock);


        if (leftNode->typeName=="worldNormal") {
            c = var.name + " = v_normal;\n";
        }
        else if (leftNode->typeName=="float") {
            c = leftVar.name + " = " + leftNode->getSocketValue(socketIndex, nullptr) + ";\n";
            auto val = convertValue(leftVar.name, outSock, inSock);
            c += var.name + " = " + val + ";\n";
        }
        else if (leftNode->typeName == "add"){
            c += evalSocket(ctx, graph, leftNode, 0);
            c += evalSocket(ctx, graph, leftNode, 1);

            // do calculations with var names
            auto a = ctx.createTempVar(leftNode->inSockets[0]);
            auto b = ctx.createTempVar(leftNode->inSockets[1]);

            c += var.name + " = " + a.name + " + " + b.name + ";\n";
        }
        else if (leftNode->typeName == "multiply"){
            c += evalSocket(ctx, graph, leftNode, 0);
            c += evalSocket(ctx, graph, leftNode, 1);

            // do calculations with var names
            auto a = ctx.createTempVar(leftNode->inSockets[0]);
            auto b = ctx.createTempVar(leftNode->inSockets[1]);

            c += var.name + " = " + a.name + " * " + b.name + ";\n";
        }

        return c;
    }

    // fromValue could be a constant or a variable name
    QString convertValue(QString fromValue, SocketModel* from, SocketModel* to)
    {
        int numFrom = getNumComponents(from->typeName);
        int numTo = getNumComponents(to->typeName);

        Q_ASSERT(numFrom!= 0 && numTo != 0);

        QString suffix = ".";

        // float being casted to a vector
        //
        if (numFrom==1 && numTo>1) {
            return QString("vec%1(%2)").arg(numTo).arg(fromValue);
        }

        if (numFrom > numTo) {
            // eg vec4 > vec2 = var.xy
            for(int i = 0; i<numTo; i++) {
                suffix += getVectorComponent(i);
            }

            return fromValue+suffix;
        }
        if (numFrom < numTo)
        {
            // eg vec2 > vec4 = var.xyyy
            for(int i = 0; i<numTo; i++) {
                suffix += getVectorComponent(qMin(i, numTo));
            }
            return fromValue+suffix;
        }

        return fromValue;

    }

    QString getVectorComponent(int index)
    {
        switch(index)
        {
            case 1: return "x";
            case 2: return "y";
            case 3: return "z";
            case 4: return "w";
        }

        return "x";
    }

    int getNumComponents(QString valueType)
    {
        if (valueType=="float")
            return 1;

        if (valueType=="vec2")
            return 2;

        if (valueType=="vec3")
            return 3;

        if (valueType=="vec4")
            return 4;

        return 0;
    }
};


NodeGraph* createGraph()
{
    NodeGraph* graph = new NodeGraph();

    // master shader node
    auto masterNode = new NodeModel();
    masterNode->typeName = "material";
    masterNode->inSockets.append(new Vector3SocketModel("Diffuse"));
    masterNode->inSockets.append(new FloatSocketModel("Alpha"));
    graph->addNode(masterNode);
    graph->setMasterNode(masterNode);

    // mult
    auto multNode = new NodeModel();
    multNode->typeName = "multiply";
    multNode->inSockets.append(new Vector3SocketModel("a"));
    multNode->inSockets.append(new Vector3SocketModel("b"));
    multNode->outSockets.append(new Vector3SocketModel("result"));
    graph->addNode(multNode);

    // normal node
    auto normalNode = new NodeModel();
    normalNode->typeName = "worldNormal";
    normalNode->outSockets.append(new Vector3SocketModel("normal"));
    graph->addNode(normalNode);

    // constant node

    // connect nodes
    graph->addConnection(multNode, 0, masterNode, 0);
    graph->addConnection(normalNode, 0, multNode, 0);

    return graph;
}


void registerModels(NodeGraph* graph)
{
    // mult
    auto multNode = new NodeModel();
    multNode->typeName = "multiply";
    multNode->title = "Multiply";
    multNode->inSockets.append(new Vector3SocketModel("a"));
    multNode->inSockets.append(new Vector3SocketModel("b"));
    multNode->outSockets.append(new Vector3SocketModel("result"));
    graph->registerModel(multNode);

    // normal node
    auto normalNode = new NodeModel();
    normalNode->typeName = "worldNormal";
    normalNode->title = "World Normal";
    normalNode->outSockets.append(new Vector3SocketModel("normal"));
    graph->registerModel(normalNode);

    auto floatNode = new FloatNodeModel();
    floatNode->outSockets.append(new FloatSocketModel("value","float"));
    graph->registerModel(floatNode);

}

QString graphTest()
{
    auto graph = createGraph();

    // eval shader
    ShaderGen shaderGen;
    auto res = shaderGen.evaluateShader(graph, graph->getMasterNode());
    return res;
}

enum class ShaderNodeType : int
{
    FloatConstant,
    Vector3Constant
};


class ShaderEvaluator
{
public:
    QString evaluate(GraphNode* rootNode, QString socketName);
};

enum class SimpleNodeType : int
{
    Five,
    Ten,
    Add,
    Mul,
    Output
};

class SimpleEvaluator
{
public:
    QString evaluate(GraphNode* rootNode, QString socketName)
    {

    }
};


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new GraphNodeScene(this);
    scene->setBackgroundBrush(QBrush(QColor(60, 60, 60)));
//    auto rect = scene->addRect(10, 10, 100, 100);
//    rect->setFlag(QGraphicsItem::ItemIsMovable);
    ui->graphicsView->setScene(scene);
    //auto node = new GraphNode(nullptr);
    //scene->addItem(node);

    /*
    // node 1
    auto node = scene->createNode<GraphNode>();
    node->addInSocket("Color");
    node->addInSocket("Multiplier");
    node->addOutSocket("Color Out");

    // node 2
    auto node2 = scene->createNode<GraphNode>();
    node2->addInSocket("Color");
    node2->addInSocket("Multiplier");
    node2->addOutSocket("Color Out");
    node2->setPos(10, 100);

    auto model = scene->createNode<GraphNode>();
    model->setTitle("Shader");
    model->addInSocket("Diffuse");
    model->addInSocket("Emission");
    model->addInSocket("Specular");
    model->addInSocket("Shininess");
    model->addInSocket("Normal");
    model->setPos(100, 100);
    */
    /*
    auto node = scene->createNode<GraphNode>();
    node->nodeType = (int)SimpleNodeType::Five;
    node->addOutSocket("Five");
    node->setPos(50, 100);

    node = scene->createNode<GraphNode>();
    node->nodeType = (int)SimpleNodeType::Ten;
    node->addOutSocket("Ten");
    node->setPos(70, 110);

    node = scene->createNode<GraphNode>();
    node->nodeType = (int)SimpleNodeType::Add;
    node->setTitle("Add");
    node->addInSocket("A");
    node->addInSocket("B");
    node->addOutSocket("Result");
    node->setPos(110, 140);

    node = scene->createNode<GraphNode>();
    node->nodeType = (int)SimpleNodeType::Mul;
    node->setTitle("Multiply");
    node->addInSocket("A");
    node->addInSocket("B");
    node->addOutSocket("Result");
    node->setPos(70, 200);

    node = scene->createNode<GraphNode>();
    node->nodeType = (int)SimpleNodeType::Output;
    node->setTitle("Result");
    node->addInSocket("Data");
    node->setPos(200, 100);
    */
    //auto code = graphTest();

    //auto graph = createGraph();
    //scene->setNodeGraph(graph);
    //ui->graphicsView->installEventFilter(this);
    //ui->textEdit->setPlainText(code);

    // set default
    auto masterNode = new NodeModel();
    masterNode->typeName = "Material";
    masterNode->inSockets.append(new Vector3SocketModel("diffuse"));
    masterNode->inSockets.append(new Vector3SocketModel("specular"));
    masterNode->inSockets.append(new FloatSocketModel("shininess"));
    masterNode->inSockets.append(new Vector3SocketModel("normal", "v_normal"));
    masterNode->inSockets.append(new Vector3SocketModel("ambient"));
    masterNode->inSockets.append(new Vector3SocketModel("emission"));
    //masterNode->inSockets.append(new FloatSocketModel("alpha"));

    auto graph = new NodeGraph;
    graph->addNode(masterNode);
    graph->setMasterNode(masterNode);
    scene->setNodeGraph(graph);

    registerModels(graph);

    connect(scene, &GraphNodeScene::newConnection, [this](SocketConnection* connection)
    {
        auto graph = scene->getNodeGraph();
        ShaderGen shaderGen;
        auto code = shaderGen.evaluateShader(graph, graph->getMasterNode());
        ui->textEdit->setPlainText(code);
        sceneWidget->updateShader(code);
    });

    connect(scene, &GraphNodeScene::nodeValueChanged, [this](NodeModel* model, int index)
    {
        auto graph = scene->getNodeGraph();
        ShaderGen shaderGen;
        auto code = shaderGen.evaluateShader(graph, graph->getMasterNode());
        ui->textEdit->setPlainText(code);
        sceneWidget->updateShader(code);
    });


    // preview widget
    sceneWidget = new SceneWidget();
    auto grid = new QGridLayout();
    grid->setSpacing(0);
    grid->setMargin(0);
    grid->addWidget(sceneWidget);
    ui->sceneContainer->setLayout(grid);
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    //return true;

    if (watched == ui->graphicsView) {
        //qDebug() << event->type();
        if (event->type() == QEvent::MouseButtonPress) {
            auto me = (QMouseEvent*)event;
            if (me->button() == Qt::RightButton) {
                // show context menu

                auto x = me->x();
                auto y = me->y();

                auto menu = new QMenu();

                connect(menu->addAction("Texture"), &QAction::triggered, [this,x, y](){

                    auto multNode = new NodeModel();
                    multNode->typeName = "multiply";
                    multNode->inSockets.append(new Vector3SocketModel("a"));
                    multNode->inSockets.append(new Vector3SocketModel("b"));
                    multNode->outSockets.append(new Vector3SocketModel("result"));
                    scene->addNodeModel(multNode, x, y);
                    //scene->activeWindow()->

                });
                menu->addAction("Add");
                menu->addAction("Multiply");

                menu->exec(mapToGlobal(QPoint(me->pos().x(), me->pos().y())));
            }
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
