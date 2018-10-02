#include "test.h"
#include "../graph/library.h"

void registerModels(NodeGraph* graph)
{
	/*
	// mult
	graph->registerModel("vectorMultiply", []()
	{
		auto multNode = new VectorMultiplyNode();
		return multNode;
	});

	// normal
	graph->registerModel("worldNormal", []()
	{
		auto normalNode = new WorldNormalNode();
		return normalNode;
	});

	// float
	graph->registerModel("float", []()
	{
		auto floatNode = new FloatNodeModel();
		return floatNode;
	});

	// time
	graph->registerModel("time", []()
	{
		auto node = new TimeNode();
		return node;
	});


	// uv
	graph->registerModel("Texture Coordinate", []()
	{
		return new TextureCoordinateNode();
	});

	// sine
	graph->registerModel("sine", []()
	{
		return new SineNode();
	});

	//make color
	graph->registerModel("Make Color", []() {
		return new MakeColorNode();
	});

	// property
	graph->registerModel("property", []()
	{
		return new PropertyNode();
	});

	*/

	auto lib = new NodeLibrary();

	// mult
	lib->addNode("vectorMultiply","Vector Multiply","", []()
	{
		auto multNode = new VectorMultiplyNode();
		return multNode;
	});

	// normal
	lib->addNode("worldNormal", "World Normal", "", []()
	{
		auto normalNode = new WorldNormalNode();
		return normalNode;
	});

	// float
	lib->addNode("float", "Float", "", []()
	{
		auto floatNode = new FloatNodeModel();
		return floatNode;
	});

	// time
	lib->addNode("time", "Time", "", []()
	{
		auto node = new TimeNode();
		return node;
	});


	// uv
	lib->addNode("texCoords", "Texture Coordinate", "", []()
	{
		return new TextureCoordinateNode();
	});

	// sine
	lib->addNode("sine", "Sine", "", []()
	{
		return new SineNode();
	});

	//make color
	lib->addNode("makeColor", "", "", []() {
		return new MakeColorNode();
	});

	graph->setNodeLibrary(lib);

	/*
	// property
	lib->addNode("property", "", "", []()
	{
		return new PropertyNode();
	});
	*/
}

SurfaceMasterNode::SurfaceMasterNode()
{
	title = "Surface Material";
	typeName = "Material";
	addInputSocket(new Vector3SocketModel("diffuse"));
	addInputSocket(new Vector3SocketModel("specular"));
	addInputSocket(new FloatSocketModel("shininess"));
	addInputSocket(new Vector3SocketModel("normal", "v_normal"));
	addInputSocket(new Vector3SocketModel("ambient"));
	addInputSocket(new Vector3SocketModel("emission"));
	addInputSocket(new FloatSocketModel("alpha"));
}

void SurfaceMasterNode::process(ModelContext* ctx)
{
	QString code = "";
	auto context = (ShaderContext*)ctx;
	//context->addCodeChunk(this, "void surface(inout Material material){\n");

	auto diffVar = this->getValueFromInputSocket(0);
	auto specVar = this->getValueFromInputSocket(1);
	auto shininessVar = this->getValueFromInputSocket(2);
	auto normVar = this->getValueFromInputSocket(3);
	auto ambientVar = this->getValueFromInputSocket(4);
	auto emissionVar = this->getValueFromInputSocket(5);
	auto alphaVar = this->getValueFromInputSocket(6);

	code += "material.diffuse = " + diffVar + ";\n";
	code += "material.specular = " + specVar + ";\n";
	code += "material.shininess = " + shininessVar + ";\n";
	code += "material.normal = " + normVar + ";\n";
	code += "material.ambient = " + ambientVar + ";\n";
	code += "material.emission = " + emissionVar + ";\n";
	code += "material.alpha = " + alphaVar + ";\n";
	//context->addCodeChunk(this, "material.diffuse = " + diffVar + ";");

	context->addCodeChunk(this, code);
}

FloatNodeModel::FloatNodeModel() :
	NodeModel()
{
	setNodeType(NodeType::Number);
	lineEdit = new QLineEdit();
	lineEdit->setValidator(new QDoubleValidator());
	lineEdit->setStyleSheet("border: 2px solid rgba(220,220,220,1);"
		"border-radius: 2px;"
		"padding: 2px 8px 2px 12px;"
		"background: rgba(0,0,0,0);"
		"color: rgba(250,250,250);");

	// lineEdit->setMaximumSize(lineEdit->sizeHint());
	// lineEdit->setMaximumSize(QSize(160,20));

	connect(lineEdit, &QLineEdit::textChanged,
		this, &FloatNodeModel::editTextChanged);

	auto containerWidget = new QWidget();
	auto layout = new QVBoxLayout;
	containerWidget->setMaximumSize(170, 55);
	containerWidget->setLayout(layout);
	containerWidget->setStyleSheet("background:rgba(0,0,0,0);");
	layout->addWidget(lineEdit);
	layout->setSpacing(0);

	this->widget = containerWidget;

	typeName = "float";
	title = "Float";

	// add output socket
	valueSock = new FloatSocketModel("value");
	addOutputSocket(valueSock);

	lineEdit->setText("0.0");
}

void FloatNodeModel::editTextChanged(const QString& text)
{
	valueSock->setValue(text);
	emit valueChanged(this, 0);
}

void FloatNodeModel::process(ModelContext* context)
{
	auto ctx = (ShaderContext*)context;
	auto res = this->getOutputSocketVarName(0);

	auto code = res + " = " + valueSock->getValue() + ";";
	//ctx->addCodeChunk(this, code);
	valueSock->setVarName(valueSock->getValue());
}

QJsonValue FloatNodeModel::serializeWidgetValue(int widgetIndex)
{
	return lineEdit->text().toDouble();
}

void FloatNodeModel::deserializeWidgetValue(QJsonValue val, int widgetIndex)
{
	auto value = val.toDouble();
	lineEdit->setText(QString("%1").arg(value));
}


VectorMultiplyNode::VectorMultiplyNode()
{
	setNodeType(NodeType::Calculation);
	title = "Vector Multiply";
	typeName = "vectorMultiply";

	addInputSocket(new Vector3SocketModel("A"));
	addInputSocket(new Vector3SocketModel("B"));
	addOutputSocket(new Vector3SocketModel("Result"));
}

void VectorMultiplyNode::process(ModelContext* context)
{
	auto ctx = (ShaderContext*)context;
	auto valA = this->getValueFromInputSocket(0);
	auto valB = this->getValueFromInputSocket(1);
	auto res = this->getOutputSocketVarName(0);

	//valA = SocketHelper::convertValue(valA,inSockets[0], outSockets[0]);
	//valB = SocketHelper::convertValue(valB,inSockets[1], outSockets[0]);

	auto code = res + " = " + valA + " * " + valB + ";";
	ctx->addCodeChunk(this, code);
}

WorldNormalNode::WorldNormalNode()
{
	setNodeType(NodeType::Surface);

	title = "World Normal";
	typeName = "worldNormal";

	addOutputSocket(new Vector3SocketModel("World Normal", "v_normal"));
}

void WorldNormalNode::process(ModelContext* context)
{
	auto ctx = (ShaderContext*)context;
	auto res = this->getOutputSocketVarName(0);

	auto code = res + " = v_normal;";
	//ctx->addCodeChunk(this, code);
	outSockets[0]->setVarName("v_normal");
}

TimeNode::TimeNode()
{
	setNodeType(NodeType::Modifier);

	title = "Time";

	addOutputSocket(new FloatSocketModel("Seconds", "u_time"));
}

void TimeNode::process(ModelContext* context)
{
	outSockets[0]->setVarName("u_time");
}

SineNode::SineNode()
{
	setNodeType(NodeType::Modifier);

	title = "Sine";

	addInputSocket(new Vector3SocketModel("Value"));
	addOutputSocket(new Vector3SocketModel("Result"));
}

void SineNode::process(ModelContext* context)
{
	auto ctx = (ShaderContext*)context;
	auto valA = this->getValueFromInputSocket(0);
	auto res = this->getOutputSocketVarName(0);

	auto code = res + " = sin(" + valA + ");";
	ctx->addCodeChunk(this, code);
}



MakeColorNode::MakeColorNode() {
	setNodeType(NodeType::Surface);


	title = "Color";
	addInputSocket(new FloatSocketModel("Value R"));
	addInputSocket(new FloatSocketModel("Value G"));
	addInputSocket(new FloatSocketModel("Value B"));

	addOutputSocket(new Vector4SocketModel("color"));
}

void MakeColorNode::process(ModelContext *context)
{
	auto ctx = (ShaderContext*)context;
	auto valr = this->getValueFromInputSocket(0);
	auto valg = this->getValueFromInputSocket(1);
	auto valb = this->getValueFromInputSocket(2);
	auto res = this->getOutputSocketVarName(0);

	auto code = res + " = vec4(" + valr + "," + valg + "," + valb + ", 1);";
	ctx->addCodeChunk(this, code);
}


TextureCoordinateNode::TextureCoordinateNode()
{
	setNodeType(NodeType::Surface);

	title = "Texture Coordinate";

	combo = new QComboBox();
	combo->addItem("TexCoord0");
	combo->addItem("TexCoord1");
	combo->addItem("TexCoord2");
	combo->addItem("TexCoord3");

	connect(combo, &QComboBox::currentTextChanged,
		this, &TextureCoordinateNode::comboTextChanged);

	combo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	combo->setStyleSheet("QComboBox{border: 2px solid rgba(250,250,250,0);color : rgba(230,230,230,1);}"
		"QComboBox QAbstractItemView{selection-background-color: lightgray; background: rgba(20,20,20,1); color : rgba(230,230,230,1); border: none; }"
		//   "QComboBox::drop-down {border: 1px solid black; }"
		"QComboBox::drop-arrow{color: rgba(230,230,230,1);}");

	auto containerWidget = new QWidget();
	auto layout = new QVBoxLayout;
	containerWidget->setMaximumSize(170, 55);
	containerWidget->setLayout(layout);
	containerWidget->setStyleSheet("background:rgba(0,0,0,0);");
	layout->addWidget(combo);
	layout->setSpacing(0);

	this->widget = containerWidget;

	//this->widget = combo;

	typeName = "float";

	addOutputSocket(new Vector2SocketModel("UV"));
	uv = "v_texCoord";
}

void TextureCoordinateNode::process(ModelContext* context)
{
	outSockets[0]->setVarName(uv);
}

void TextureCoordinateNode::comboTextChanged(const QString& text)
{
	if (text == "TexCoord0") {
		uv = "v_texCoord";
	}
	else if (text == "TexCoord1") {
		uv = "v_texCoord1";
	}
	else if (text == "TexCoord2") {
		uv = "v_texCoord2";
	}
	else if (text == "TexCoord3") {
		uv = "v_texCoord3";
	}

	emit valueChanged(this, 0);
}


PropertyNode::PropertyNode()
{
	this->typeName = "property";
}

// doesnt own property
void PropertyNode::setProperty(Property* property)
{
	this->prop = property;
	this->title = property->displayName;

	// add output based on property type
	switch (property->type) {
	case PropertyType::Int:
		this->addOutputSocket(new FloatSocketModel("int"));
		break;
	case PropertyType::Float:
		this->addOutputSocket(new FloatSocketModel("float"));
		break;

	case PropertyType::Vec2:
		this->addOutputSocket(new Vector2SocketModel("vector2"));
		break;

	case PropertyType::Vec3:
		this->addOutputSocket(new Vector3SocketModel("vector3"));
		break;

	case PropertyType::Vec4:
		this->addOutputSocket(new Vector4SocketModel("vector4"));
		break;

	default:
		//todo: throw error or something
		Q_ASSERT(false);
		break;
	}
}

QJsonValue PropertyNode::serializeWidgetValue(int widgetIndex)
{
	if (this->prop) {
		return this->prop->id;
	}

	return "";
}

void PropertyNode::process(ModelContext* context)
{
	auto ctx = (ShaderContext*)context;
	ctx->addUniform(prop->getUniformString());

	if (this->outSockets.count() > 0)
		outSockets[0]->setVarName(prop->getUniformName());
}