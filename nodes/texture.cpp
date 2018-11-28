#include "texture.h"

/*    COMBINE NORMAL    */
CombineNormalsNode::CombineNormalsNode()
{
	setNodeType(NodeType::Math);
	title = "Combine Normals";
	typeName = "combinenormals";
	enablePreview = true;

	addInputSocket(new Vector3SocketModel("NormalA"));
	addInputSocket(new Vector3SocketModel("NormalB"));
	addOutputSocket(new Vector3SocketModel("Result"));
}

void CombineNormalsNode::process(ModelContext* context)
{
	auto ctx = (ShaderContext*)context;
	auto normalA = this->getValueFromInputSocket(0);
	auto normalB = this->getValueFromInputSocket(1);
	auto res = this->getOutputSocketVarName(0);

	auto code = res + " = normalize((" + normalA + " + " + normalB + ")*2 - 2)";
	ctx->addCodeChunk(this, code);
}

QString CombineNormalsNode::generatePreview(ModelContext* context)
{
	auto ctx = (ShaderContext*)context;
	auto normalA = this->getValueFromInputSocket(0);
	auto normalB = this->getValueFromInputSocket(1);
	auto res = this->getOutputSocketVarName(0);

	auto output = "preview.color = normalize((" + normalA + " + " + normalB + ")*2 - 2)";

	return output;
}


TexelSizeNode::TexelSizeNode()
{
	setNodeType(NodeType::Math);
	title = "Texel Size";
	typeName = "texelsize";
	enablePreview = true;

	addInputSocket(new TextureSocketModel("Texture"));
	addOutputSocket(new Vector2SocketModel("Size"));
	addOutputSocket(new FloatSocketModel("Width"));
	addOutputSocket(new FloatSocketModel("Height"));
	addOutputSocket(new FloatSocketModel("1/Width"));
	addOutputSocket(new FloatSocketModel("1/Height"));
}

void TexelSizeNode::process(ModelContext* context)
{
	auto texName = this->getValueFromInputSocket(0);
	this->outSockets[0]->setVarName("textureSize(" + texName + ", 0.0)");
	this->outSockets[1]->setVarName("textureSize(" + texName + ", 0.0).x");
	this->outSockets[2]->setVarName("textureSize(" + texName + ", 0.0).y");
	this->outSockets[3]->setVarName("(1.0 / textureSize(" + texName + ", 0.0).x)");
	this->outSockets[4]->setVarName("(1.0 / textureSize(" + texName + ", 0.0).y)");
}