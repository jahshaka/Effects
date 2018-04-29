#include "nodegraph.h"
#include "nodemodel.h"
#include <QMouseEvent>
#include <QDebug>
#include "scenewidget.h"
#include <QLayout>
#include <QGridLayout>
#include <QLineEdit>

// temporary variable
struct TempVar
{
    QString name;
    QString typeName;
};

struct CodeChunk
{
    //NodeModel* node = nullptr;
    QString owner;
    QString code;
};

class ShaderContext : public ModelContext
{
    // mapped using the socket guid
    QMap<QString, TempVar> tempVars;
    QList<CodeChunk> codeChunks;
    //QString code;
    bool debugCode;
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

    void clear()
    {
        tempVars.clear();
    }

    void addCodeChunk(NodeModel* node, QString code)
    {
        QString ownerName = node->title + " - " + node->id;
        //CodeChunk chunk = {node, code};
        codeChunks.append({ownerName, code});
    }

    QString generateVars()
    {
        QString finalCode = "";

        // generate temp vars
        for(auto& var : tempVars) {
            auto c = var.typeName + " " + var.name + ";\n";
            finalCode = c + finalCode;
        }

        return finalCode;
    }

    QString generateCode(bool debug = false)
    {
        QString finalCode = "";

        // combine code chunks
        for (auto chunk : codeChunks) {
            if (debug)
                finalCode += "// "+chunk.owner+"\n";

            finalCode += chunk.code;
            finalCode += "\n";

            // extra space for debug mode
            if (debug)
                finalCode += "\n";
        }

        return finalCode;
    }
};

class ShaderGenerator
{
public:
    QString generateShader(NodeGraph* graph)
    {
        auto ctx = new ShaderContext();

        auto masterNode = graph->getMasterNode();

        generateProperties(graph, ctx);
        processNode(masterNode, ctx);

        auto code = ctx->generateVars();

        code += "void surface(inout Material material){\n";
        code += ctx->generateCode(true);
        code += "}\n";

        return code;
    }

    void generateProperties(NodeGraph* graph, ShaderContext* ctx)
    {
        for(auto node : graph->nodes.values()) {
            for(auto sock : node->outSockets) {
                auto tempVar = ctx->createTempVar(sock);
                sock->setVarName(tempVar.name);
            }
        }
    }

    void preprocess();

    void processNode(NodeModel* node, ShaderContext* ctx)
    {
        for ( auto sock : node->inSockets) {
            if (sock->hasConnection()) {
                auto con = sock->getConnection();
                processNode(con->leftSocket->getNode(), ctx);
            }
        }

        node->process(ctx);
    }

    void postprocess();
};


class SurfaceMasterNode : public NodeModel
{
public:
    SurfaceMasterNode()
    {
        typeName = "Material";
        addInputSocket(new Vector3SocketModel("diffuse"));
        addInputSocket(new Vector3SocketModel("specular"));
        addInputSocket(new FloatSocketModel("shininess"));
        addInputSocket(new Vector3SocketModel("normal", "v_normal"));
        addInputSocket(new Vector3SocketModel("ambient"));
        addInputSocket(new Vector3SocketModel("emission"));
        addInputSocket(new FloatSocketModel("alpha"));
    }

    virtual void process(ModelContext* ctx) override
    {
        auto context = (ShaderContext*)ctx;
        //context->addCodeChunk(this, "void surface(inout Material material){\n");

        auto diffVar = this->getValueFromInputSocket(0);
        context->addCodeChunk(this, "material.diffuse = " + diffVar + ";");

        //context->addCodeChunk(this, "}");
    }
};

class FloatNodeModel : public NodeModel
{
    QLineEdit* lineEdit;

    FloatSocketModel* valueSock;
public:
    FloatNodeModel():
        NodeModel()
    {
        lineEdit = new QLineEdit();
        lineEdit->setValidator(new QDoubleValidator());
        lineEdit->setMaximumSize(lineEdit->sizeHint());

        connect(lineEdit, &QLineEdit::textChanged,
                  this, &FloatNodeModel::editTextChanged);

        this->widget = lineEdit;

        typeName = "float";
        title = "Float";

        // add output socket
        valueSock = new FloatSocketModel("value");
        addOutputSocket(valueSock);

        lineEdit->setText("0.0");
    }

    void editTextChanged(const QString& text)
    {
        valueSock->setValue(text);
        emit valueChanged(this, 0);
    }

    virtual void process(ModelContext* context) override
    {
        auto ctx = (ShaderContext*)context;
        auto res = this->getOutputSocketVarName(0);

        auto code = res +" = "+valueSock->getValue()+";";
        //ctx->addCodeChunk(this, code);
        valueSock->setVarName(valueSock->getValue());
    }
};


class VectorMultiplyNode : public NodeModel
{
public:
    VectorMultiplyNode()
    {
        title = "Vector Multiply";

        addInputSocket(new Vector3SocketModel("A"));
        addInputSocket(new Vector3SocketModel("B"));
        addOutputSocket(new Vector3SocketModel("Result"));
    }

    virtual void process(ModelContext* context) override
    {
        auto ctx = (ShaderContext*)context;
        auto valA = this->getValueFromInputSocket(0);
        auto valB = this->getValueFromInputSocket(1);
        auto res = this->getOutputSocketVarName(0);

        auto code = res +" = " + valA + " * " + valB + ";";
        ctx->addCodeChunk(this, code);
    }
};

class WorldNormalNode : public NodeModel
{
public:
    WorldNormalNode()
    {
        title = "World Normal";

        addOutputSocket(new Vector3SocketModel("World Normal","v_normal"));
    }

    virtual void process(ModelContext* context) override
    {
        auto ctx = (ShaderContext*)context;
        auto res = this->getOutputSocketVarName(0);

        auto code = res +" = v_normal;";
        //ctx->addCodeChunk(this, code);
        outSockets[0]->setVarName("v_normal");
    }
};

void registerModels(NodeGraph* graph)
{
    // mult
    graph->registerModel("Multiply", []()
    {
        auto multNode = new VectorMultiplyNode();
        return multNode;
    });

    // normal
    graph->registerModel("World Normal", []()
    {
        auto normalNode = new WorldNormalNode();
        return normalNode;
    });

    // float
    graph->registerModel("Float", []()
    {
        auto floatNode = new FloatNodeModel();
        return floatNode;
    });

}



/*
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

QString graphTest()
{
    auto graph = createGraph();

    // eval shader
    ShaderGen shaderGen;
    auto res = shaderGen.evaluateShader(graph, graph->getMasterNode());
    return res;
}

*/
