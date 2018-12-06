#include "test.h"
#include "../graph/library.h"
#include "../texturemanager.h"
#include "../propertywidgets/vectorpropertywidget.h"
#include <QFileDialog>
#include <QDebug>
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
	
	// pulsate
	lib->addNode("pulsate", "Pulsate Node", "", []()
	{
		return new PulsateNode();
	});

	//make color
	lib->addNode("makeColor", "Make Color", "", []() {
		return new MakeColorNode();
	});

	//sample texture
	lib->addNode("sampleTexture", "Sample Texture", "", []() {
		return new TextureSamplerNode();
	});

	//texture
	lib->addNode("texture", "Texture", "", []() {
		return new TextureNode();
	});

	lib->addNode("vector2", "Vector2", "", []() {
		return new Vector2Node();
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
	setNodeType(NodeType::Input);
	addInputSocket(new Vector3SocketModel("Diffuse","vec3(1.0,1.0,1.0)"));
	addInputSocket(new Vector3SocketModel("Specular"));
	addInputSocket(new FloatSocketModel("Shininess"));
	addInputSocket(new Vector3SocketModel("Normal", "v_normal"));
	addInputSocket(new Vector3SocketModel("Ambient"));
	addInputSocket(new Vector3SocketModel("Emission"));
	addInputSocket(new FloatSocketModel("Alpha"));
	addInputSocket(new FloatSocketModel("Alpha Cutoff"));
	addInputSocket(new Vector3SocketModel("Vertex Offset"));
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
	auto alphaCutoffVar = this->getValueFromInputSocket(7);

	code += "material.diffuse = " + diffVar + ";\n";
	code += "material.specular = " + specVar + ";\n";
	code += "material.shininess = " + shininessVar + ";\n";
	code += "material.normal = " + normVar + ";\n";
	code += "material.ambient = " + ambientVar + ";\n";
	code += "material.emission = " + emissionVar + ";\n";
	code += "material.alpha = " + alphaVar + ";\n";
	//code += "material.alphaCutoff = " + alphaCutoffVar + ";\n";
	//context->addCodeChunk(this, "material.diffuse = " + diffVar + ";");

	//context->clear();
	context->addCodeChunk(this, code);
	
}

FloatNodeModel::FloatNodeModel() :
	NodeModel()
{
	setNodeType(NodeType::Constants);

	auto wid = new QWidget;
	auto label = new QLabel(" ");
	auto layout = new QVBoxLayout;
	auto layoutH = new QHBoxLayout;
	auto layoutH1 = new QHBoxLayout;
	auto layoutH2 = new QHBoxLayout;
	auto layoutH3 = new QHBoxLayout;
	auto min = new QDoubleSpinBox;
	auto max = new QDoubleSpinBox;
	auto stepBox = new QDoubleSpinBox;
	auto valueBox = new QDoubleSpinBox;

	min->setValue(0.0);
	max->setValue(1.0);
	stepBox->setValue(0.1);

	double step = 0.1;
	double upModifier = (step * 1000);

	auto slider = new QSlider(Qt::Horizontal);
	slider->setMinimum(min->value());
	slider->setMaximum(max->value()*upModifier);
	slider->setSingleStep(stepBox->value());

	//min->setMaximumWidth(50);
	//max->setMaximumWidth(50);
	//valueBox->setMaximumWidth(50);
	min->setAlignment(Qt::AlignCenter);
	max->setAlignment(Qt::AlignCenter);
	valueBox->setAlignment(Qt::AlignCenter);
	slider->setMaximumWidth(150);
	
	

	layout->setSpacing(0);
	layoutH->setSpacing(0);
	wid->setLayout(layout);

//	wid->setMaximumWidth(200);

	//layoutH->addWidget(new QLabel("Min"));
	layoutH->addWidget(min);

//	layoutH1->addWidget(new QLabel("Max"));
	layoutH->addWidget(max);
//	layoutH2->addWidget(new QLabel("Step"));
//	layoutH2->addWidget(stepBox);
//	layoutH3->addWidget(new QLabel("Value"));
	layoutH->addWidget(valueBox);

	auto holdem = new QWidget;
	holdem->setLayout(layoutH);


	auto sliderHolder = new QWidget;
	auto sliderLayout = new QHBoxLayout;
	sliderHolder->setLayout(sliderLayout);
	sliderLayout->addWidget(slider);
	auto minl = new QLabel("min");
	auto maxl = new QLabel(" max");
	auto vall = new QLabel("value");

	minl->setAlignment(Qt::AlignCenter);
	maxl->setAlignment(Qt::AlignCenter);
	vall->setAlignment(Qt::AlignCenter);

	auto textHolder = new QWidget;
	//textHolder->setMaximumWidth(150);
	textHolder->setLayout(layoutH1);
	layoutH1->setContentsMargins(10, 0, 10, 0);


	layoutH1->addWidget(minl);
		layoutH1->addWidget(maxl);
		layoutH1->addWidget(vall);

	layout->addWidget(textHolder);

	//layout->addLayout(layoutH);
	layout->addWidget(holdem);
	/*layout->addLayout(layoutH1);
	layout->addLayout(layoutH2);
	layout->addLayout(layoutH3);*/
	layout->addWidget(sliderHolder);
	
	this->widget = wid;

	typeName = "float";
	title = "Float";

	// add output socket
	valueSock = new FloatSocketModel("value");
	addOutputSocket(valueSock);
	widget->setMaximumWidth(164);
	

	connect(min, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double val) {
		slider->setMinimum(val);
	});
	connect(max, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double val) {
		slider->setMaximum(val*upModifier);
	}); 
	connect(valueBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double val) {
		slider->blockSignals(true);
		slider->setValue(val*step);
		slider->blockSignals(false);
	});
	connect(stepBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [&step, &upModifier](double val) {
		step = val;
		upModifier = (step * 100);
	});
	connect(slider, &QSlider::valueChanged, [=](int value) {
		editTextChanged(QString::number(value/upModifier, 'g', 2));
		valueBox->blockSignals(true);
		valueBox->setValue(qreal(value/upModifier));
		qDebug() << qreal(value / upModifier);
		valueBox->blockSignals(false);

	});


	wid->setStyleSheet(""
		"QWidget{ background: rgba(0,0,0,0); color: rgba(250,250,250,1); }"

		"");
	min->setStyleSheet("QDoubleSpinBox{border: 2px solid rgba(0, 0, 0, .4); "
		"border-radius: 0px;"
		"padding: 2px 2px 2px 2px;"
		"background: rgba(0,0,0,0.2);"
		"color: rgba(250,250,250);}"
		"QDoubleSpinBox::up-arrow, QDoubleSpinBox::down-arrow { width: 0; height:0;}"
		"QDoubleSpinBox::up-button, QDoubleSpinBox::down-button { width: 0; height:0;}");
	max->setStyleSheet(min->styleSheet());
	stepBox->setStyleSheet(min->styleSheet());
	valueBox->setStyleSheet(min->styleSheet());

//	lineEdit->setText("0.0");
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
	return valueSock->getValue().toDouble();
}

void FloatNodeModel::deserializeWidgetValue(QJsonValue val, int widgetIndex)
{
	auto value = val.toDouble();
	lineEdit->setText(QString("%1").arg(value));
}


VectorMultiplyNode::VectorMultiplyNode()
{
	setNodeType(NodeType::Input);
	title = "Vector Multiply";
	typeName = "vectorMultiply";

	addInputSocket(new Vector4SocketModel("A"));
	addInputSocket(new Vector4SocketModel("B"));
	addOutputSocket(new Vector4SocketModel("Result"));
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
	setNodeType(NodeType::Input);

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

LocalNormalNode::LocalNormalNode()
{
	setNodeType(NodeType::Input);

	title = "Local Normal";
	typeName = "localNormal";

	addOutputSocket(new Vector3SocketModel("Local Normal", "v_locaNormal"));
}

void LocalNormalNode::process(ModelContext* context)
{
	auto ctx = (ShaderContext*)context;
	auto res = this->getOutputSocketVarName(0);

	outSockets[0]->setVarName("v_localNormal");
}

TimeNode::TimeNode()
{
	setNodeType(NodeType::Input);

	title = "Time";
	typeName = "time";

	addOutputSocket(new FloatSocketModel("Seconds", "u_time"));
}

void TimeNode::process(ModelContext* context)
{
	outSockets[0]->setVarName("u_time");
}

SineNode::SineNode()
{
	setNodeType(NodeType::Input);

	title = "Sine";
	typeName = "sine";

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
	setNodeType(NodeType::Math);


	title = "Color";
	typeName = "makeColor";

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
	setNodeType(NodeType::Input);

	title = "Texture Coordinate";
	typeName = "texCoords";

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


TextureSamplerNode::TextureSamplerNode()
{
	setNodeType(NodeType::Input);


	title = "Sample Texture";
	typeName = "textureSampler";

	addInputSocket(new TextureSocketModel("Texture"));
	addInputSocket(new Vector2SocketModel("UV","v_texCoord"));
	addOutputSocket(new Vector4SocketModel("RGBA"));
}

void TextureSamplerNode::process(ModelContext* context)
{
	auto ctx = (ShaderContext*)context;

	auto tex = this->getValueFromInputSocket(0);
	auto uv = this->getValueFromInputSocket(1);
	auto rgba = this->getOutputSocketVarName(0);

	QString code = "";

	if (tex.isEmpty())
		code = rgba + " = vec4(0.0,0.0,0.0,0.0);";
	else
		code = rgba + " = texture("+tex+","+uv+");";

	ctx->addCodeChunk(this, code);
}

PropertyNode::PropertyNode()
{
	this->typeName = "property";
	setNodeType(NodeType::Input);
}

// doesnt own property
void PropertyNode::setProperty(Property* property)
{
	this->prop = property;
	this->title = property->displayName;

	// add output based on property type
	switch (property->type) {
	case PropertyType::Int: {
		this->addOutputSocket(new FloatSocketModel("int"));
		this->widget = new WidgetInt;
		updateStyle();
	}
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

	case PropertyType::Texture:
		this->addOutputSocket(new TextureSocketModel("texture"));
		this->addOutputSocket(new Vector4SocketModel("rgba"));
		/*
		this->addOutputSocket(new FloatSocketModel("r"));
		this->addOutputSocket(new FloatSocketModel("g"));
		this->addOutputSocket(new FloatSocketModel("b"));
		this->addOutputSocket(new FloatSocketModel("a"));
		*/
		this->addInputSocket(new Vector2SocketModel("uv","v_texCoord"));
		
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

	if (prop->type == PropertyType::Texture) {
		auto uv = this->getValueFromInputSocket(0);
		auto rgba = this->getOutputSocketVarName(1);

		QString code = "";
		code = rgba + " = texture(" + prop->getUniformName() + "," + uv + ");";
		ctx->addCodeChunk(this, code);
	}
}


TexturePropertyNode::TexturePropertyNode()
{

}

// doesnt own property
void TexturePropertyNode::setProperty(Property* property)
{

}

QJsonValue TexturePropertyNode::serializeWidgetValue(int widgetIndex)
{
	return QJsonValue();
}

void TexturePropertyNode::process(ModelContext* context)
{

}

TextureNode::TextureNode()
{
	setNodeType(NodeType::Input);
	title = "Texture";
	typeName = "texture";

	auto widget = new QWidget;
	auto layout = new QVBoxLayout;
	widget->setLayout(layout);
	widget->setMinimumSize(170, 155);
	texture = new QPushButton();
	texture->setIconSize(QSize(145, 145));
	texture->setMinimumSize(160, 146);
	

	auto label = new QLabel;

	auto pushLayout = new QVBoxLayout;
	texture->setLayout(pushLayout);
	//pushLayout->addWidget(label);

	layout->setContentsMargins(3, 0, 3, 2);
	layout->addWidget(texture);
	this->widget = widget;

	graphTexture = nullptr;
	connect(texture, &QPushButton::clicked, [=]() {
		auto filename = QFileDialog::getOpenFileName();
		QIcon icon(filename);
		texture->setIcon(icon);

		if (graphTexture != nullptr) {
			TextureManager::getSingleton()->removeTexture(graphTexture);
			delete graphTexture;
		}

		graphTexture = TextureManager::getSingleton()->createTexture();
		graphTexture->path = filename;
	});
	
	widget->setStyleSheet("background:rgba(0,0,0,0); color: rgba(250,250,250,.9);");
	texture->setStyleSheet("background:rgba(0,0,0,0); border : 2px solid rgba(50,50,50,.3);");


	addOutputSocket(new TextureSocketModel("texture"));

}

void TextureNode::process(ModelContext * context)
{
	auto ctx = (ShaderContext*)context;
	auto texName = this->getOutputSocketVarName(0);
	ctx->addUniform("uniform sampler2D "+texName+"");
	if (graphTexture != nullptr) {
		graphTexture->uniformName = texName;
	}
}

PulsateNode::PulsateNode()
{
	setNodeType(NodeType::Math);

	title = "Pulsate";
	typeName = "pulsate";

	addInputSocket(new FloatSocketModel("Speed", "1.0"));
	addOutputSocket(new FloatSocketModel("Result"));
}

void PulsateNode::process(ModelContext * context)
{
	auto ctx = (ShaderContext*)context;
	auto valA = this->getValueFromInputSocket(0);
	auto res = this->getOutputSocketVarName(0);

	auto code = res + " = sin(u_time *" + valA + ") * 0.5 + 0.5;";
	ctx->addCodeChunk(this, code);
}

PannerNode::PannerNode()
{
	setNodeType(NodeType::Input);

	title = "Panner";
	typeName = "panner";

	addInputSocket(new Vector2SocketModel("UV", "v_texCoord"));
	addInputSocket(new Vector2SocketModel("Speed", "vec2(1.0,1.0)"));
	addInputSocket(new FloatSocketModel("Time", "u_time"));
	addOutputSocket(new Vector2SocketModel("Result"));
}

void PannerNode::process(ModelContext * context)
{
	auto ctx = (ShaderContext*)context;
	auto uv = this->getValueFromInputSocket(0);
	auto speed = this->getValueFromInputSocket(1);
	auto time = this->getValueFromInputSocket(2);
	auto res = this->getOutputSocketVarName(0);

	auto code = res + " = " + uv + " + " + speed + " * "+ time + ";";
	ctx->addCodeChunk(this, code);
}

Vector2Node::Vector2Node()
{
	setNodeType(NodeType::Constants);
	title = "Vector 2 Node";
	typeName = "vector 2 node";

	x = y = 0;

	auto wid = new QWidget;
	auto layout = new QHBoxLayout;
	wid->setLayout(layout);
	wid->setMaximumWidth(164);

	auto xSpinBox = new QDoubleSpinBox;
	auto ySpinBox = new QDoubleSpinBox;
	xSpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	ySpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	ySpinBox->setAlignment(Qt::AlignCenter);
	ySpinBox->setAlignment(Qt::AlignCenter);

	layout->addWidget(xSpinBox);
	layout->addWidget(ySpinBox);

	widget = wid;

	connect(xSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double val) {
		x = val;
		value = QVector2D(x, y);
		emit valueChanged(this, 0);

	});

	connect(ySpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double val) {
		y = val;
		value = QVector2D(x, y);
		emit valueChanged(this, 0);
	});

	widget->setStyleSheet("QDoubleSpinBox{border: 2px solid rgba(0, 0, 0, .4); padding: 2px; background: rgba(0, 0, 0, 0.2);}" 
		"QWidget{ background: rgba(0,0,0,0); color: rgba(250,250,250,1); }"
		"QDoubleSpinBox::up-arrow, QDoubleSpinBox::down-arrow { width: 0; height:0;}"
		"QDoubleSpinBox::up-button, QDoubleSpinBox::down-button { width: 0; height:0;}");


	addOutputSocket(new Vector2SocketModel("Result"));

}

void Vector2Node::process(ModelContext * context)
{
	auto ctx = (ShaderContext*)context;
	outSockets[0]->setVarName("vec2(" + QString::number(value.x()) + "," + QString::number(value.y()) + ")");
}

Vector3Node::Vector3Node()
{
	setNodeType(NodeType::Constants);
	title = "Vector 3 Node";
	typeName = "vector 3 node";

	x = y = z = 0;

	auto wid = new QWidget;
	auto layout = new QHBoxLayout;
	wid->setLayout(layout);
	wid->setMaximumWidth(164);

	auto xSpinBox = new QDoubleSpinBox;
	auto ySpinBox = new QDoubleSpinBox;
	auto zSpinBox = new QDoubleSpinBox;
	xSpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	ySpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	zSpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	ySpinBox->setAlignment(Qt::AlignCenter);
	ySpinBox->setAlignment(Qt::AlignCenter);
	zSpinBox->setAlignment(Qt::AlignCenter);

	layout->addWidget(xSpinBox);
	layout->addWidget(ySpinBox);
	layout->addWidget(zSpinBox);

	widget = wid;

	connect(xSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double val) {
		x = val;
		value = QVector3D(x, y, z);
		emit valueChanged(this, 0);

	});

	connect(ySpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double val) {
		y = val;
		value = QVector3D(x, y, z);
		emit valueChanged(this, 0);
	});

	connect(zSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double val) {
		z = val;
		value = QVector3D(x, y, z);
		emit valueChanged(this, 0);
	});

	widget->setStyleSheet("QDoubleSpinBox{border: 2px solid rgba(0, 0, 0, .4); padding: 2px; background: rgba(0, 0, 0, 0.2);}"
		"QWidget{ background: rgba(0,0,0,0); color: rgba(250,250,250,1); }"
		"QDoubleSpinBox::up-arrow, QDoubleSpinBox::down-arrow { width: 0; height:0;}"
		"QDoubleSpinBox::up-button, QDoubleSpinBox::down-button { width: 0; height:0;}");


	addOutputSocket(new Vector3SocketModel("Result"));
}

void Vector3Node::process(ModelContext * context)
{
	auto ctx = (ShaderContext*)context;
	outSockets[0]->setVarName("vec3(" + QString::number(value.x()) + "," + QString::number(value.y()) + "," + QString::number(value.z()) + ")");
}

Vector4Node::Vector4Node()
{
	setNodeType(NodeType::Constants);
	title = "Vector 4 Node";
	typeName = "vector 4 node";

	x = y = z = w = 0;

	auto wid = new QWidget;
	auto layout = new QHBoxLayout;
	wid->setLayout(layout);
	wid->setMaximumWidth(164);

	auto xSpinBox = new QDoubleSpinBox;
	auto ySpinBox = new QDoubleSpinBox;
	auto zSpinBox = new QDoubleSpinBox;
	auto wSpinBox = new QDoubleSpinBox;
	xSpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	ySpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	zSpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	wSpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	ySpinBox->setAlignment(Qt::AlignCenter);
	ySpinBox->setAlignment(Qt::AlignCenter);
	zSpinBox->setAlignment(Qt::AlignCenter);
	wSpinBox->setAlignment(Qt::AlignCenter);

	layout->addWidget(xSpinBox);
	layout->addWidget(ySpinBox);
	layout->addWidget(zSpinBox);
	layout->addWidget(wSpinBox);

	widget = wid;

	connect(xSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double val) {
		x = val;
		value = QVector4D(x, y, z, w);
		emit valueChanged(this, 0);

	});

	connect(ySpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double val) {
		y = val;
		value = QVector4D(x, y, z, w);
		emit valueChanged(this, 0);
	});

	connect(zSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double val) {
		z = val;
		value = QVector4D(x, y, z, w);
		emit valueChanged(this, 0);
	});

	connect(zSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double val) {
		z = val;
		value = QVector4D(x, y, z, w);
		emit valueChanged(this, 0);
	});

	widget->setStyleSheet("QDoubleSpinBox{border: 2px solid rgba(0, 0, 0, .4); padding: 2px; background: rgba(0, 0, 0, 0.2);}"
		"QWidget{ background: rgba(0,0,0,0); color: rgba(250,250,250,1); }"
		"QDoubleSpinBox::up-arrow, QDoubleSpinBox::down-arrow { width: 0; height:0;}"
		"QDoubleSpinBox::up-button, QDoubleSpinBox::down-button { width: 0; height:0;}");


	addOutputSocket(new Vector4SocketModel("Result"));
}

void Vector4Node::process(ModelContext * context)
{
	auto ctx = (ShaderContext*)context;
	outSockets[0]->setVarName("vec4(" + QString::number(value.x()) + "," + QString::number(value.y()) + "," + QString::number(value.z()) + "," + QString::number(value.w()) + ")");
}

#if(EFFECT_BUILD_AS_LIB)
ColorPickerNode::ColorPickerNode()
{
	setNodeType(NodeType::Constants);
	title = "Color Node";
	typeName = "Color node";

	colorWidget = new ColorPickerWidget();
	this->widget = colorWidget;
	colorWidget->setGeometry(0, 0, 60, 140);
	connect(colorWidget, &ColorPickerWidget::onColorChanged, [=](QColor color) {
		emit valueChanged(this, 0);
	});

	addOutputSocket(new Vector4SocketModel("R G B A"));
	addOutputSocket(new FloatSocketModel("R"));
	addOutputSocket(new FloatSocketModel("G"));
	addOutputSocket(new FloatSocketModel("B"));
	addOutputSocket(new FloatSocketModel("A"));
	//addOutputSocket(new FloatSocketModel("R"));
}

void ColorPickerNode::process(ModelContext * context)
{
	auto col = colorWidget->getColor();
	outSockets[0]->setVarName("vec4(" + QString::number(col.redF()) + "," + QString::number(col.greenF()) + "," + QString::number(col.blueF()) + "," + QString::number(col.alphaF()) + ")");
	outSockets[1]->setVarName(QString::number(col.redF()));
	outSockets[2]->setVarName(QString::number(col.greenF()));
	outSockets[3]->setVarName(QString::number(col.blueF()));
	outSockets[4]->setVarName(QString::number(col.alphaF()));

}
#endif