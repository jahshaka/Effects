#include "libraryv1.h"
#include "test.h"
#include "math.h"
#include "object.h"
#include "vector.h"
#include "texture.h"


void LibraryV1::init()
{
	initTest();
	initMath();
	initObject();
	initVector();
	initTexture();
	initConstants();
	initUtility();
}

void LibraryV1::initTest()
{
	auto lib = this;
	auto iconPath = ":/icons/input.png";

	// property
	lib->addNode("property", "Property", iconPath, []()
	{
		auto node = new PropertyNode();
		return node;
	});

	// mult
	lib->addNode("vectorMultiply", "Vector Multiply", iconPath, []()
	{
		auto multNode = new VectorMultiplyNode();
		return multNode;
	});

	// normal
	lib->addNode("worldNormal", "World Normal", iconPath, []()
	{
		auto normalNode = new WorldNormalNode();
		return normalNode;
	});

	// normal
	lib->addNode("localNormal", "Local Normal", iconPath, []()
	{
		auto normalNode = new LocalNormalNode();
		return normalNode;
	});

	// time
	lib->addNode("time", "Time", iconPath, []()
	{
		auto node = new TimeNode();
		return node;
	});


	// uv
	lib->addNode("texCoords", "Texture Coordinate", iconPath, []()
	{
		return new TextureCoordinateNode();
	});

	//sample texture
	lib->addNode("textureSampler", "Sample Texture", iconPath, []() {
		return new TextureSamplerNode();
	});

	// texture
	lib->addNode("texture", "Texture", iconPath, []() {
		return new TextureNode();
	});

	// panner
	lib->addNode("panner", "Panner", iconPath, []() {
		return new PannerNode();
	});
	
	// normal intensity
	lib->addNode("normalintensity", "Normal Intensity", iconPath, []() {
		return new NormalIntensityNode();
	});
}

void LibraryV1::initObject()
{
	auto iconPath = ":/icons/object.png";
	addNode("fresnel", "Fresnel", iconPath,[]() {
		return new FresnelNode();
	});

	addNode("depth", "Depth", iconPath,[]() {
		return new DepthNode();
	});
}

void LibraryV1::initMath()
{
	auto iconPath = ":/icons/math.png";

	addNode("add", "Add", iconPath, []() {
		return new AddNode();
	});

	addNode("subtract", "Subtract", iconPath, []() {
		return new SubtractNode();
	});

	addNode("multiply", "Multiply", iconPath, []() {
		return new MultiplyNode();
	});

	addNode("divide", "Divide", iconPath, []() {
		return new DivideNode();
	});

	/*
	addNode(iconPath, iconPath, iconPath, []() {
		return new Node();
	});
	*/

	// sine
	addNode("sine", "Sine", iconPath, []()
	{
		return new SineNode();
	});

	addNode("power", "Power", iconPath, []() {
		return new PowerNode();
	});

	addNode("sqrt", "Square Root", iconPath, []() {
		return new SqrtNode();
	});

	addNode("min", "Min", iconPath, []() {
		return new MinNode();
	});

	addNode("max", "Max", iconPath, []() {
		return new MaxNode();
	});

	addNode("abs", "Abs", iconPath, []() {
		return new AbsNode();
	});

	addNode("sign", "Sign", iconPath, []() {
		return new SignNode();
	});

	addNode("ceil", "Ceil", iconPath, []() {
		return new CeilNode();
	});

	addNode("floor", "Floor", iconPath, []() {
		return new FloorNode();
	});

	addNode("round", "Round", iconPath, []() {
		return new RoundNode();
	});

	addNode("trunc", "Truncate", iconPath, []() {
		return new TruncNode();
	});

	addNode("step", "Step", iconPath, []() {
		return new StepNode();
	});

	addNode("smoothstep", "Smooth Step", iconPath, []() {
		return new SmoothStepNode();
	});

	addNode("frac", "Fraction", iconPath, []() {
		return new FracNode();
	});

	addNode("clamp", "Clamp", iconPath, []() {
		return new ClampNode();
	});

	addNode("lerp", "Lerp", iconPath, []() {
		return new LerpNode();
	});

	addNode("oneminus", "One Minus", iconPath, []() {
		return new OneMinusNode();
	});

	addNode("negate", "Negate", iconPath, []() {
		return new NegateNode();
	});

	
}

void LibraryV1::initVector()
{
	auto iconPath = ":/icons/vector.png";
	addNode("reflect", "Reflect", iconPath, []() {
		return new ReflectVectorNode();
	});

	addNode("splitvector", "Split Vector", iconPath, []() {
		return new SplitVectorNode();
	});

	addNode("composevector", "Compose Vector", iconPath, []() {
		return new ComposeVectorNode();
	});

	addNode("distance", "Distance", iconPath, []() {
		return new DistanceVectorNode();
	});

	addNode("dot", "Dot", iconPath, []() {
		return new DotVectorNode();
	});

	addNode("length", "Length", iconPath, []() {
		return new LengthVectorNode();
	});

	addNode("normalize", "Normalize", iconPath, []() {
		return new NormalizeVectorNode();

	});
}

void LibraryV1::initTexture()
{
	auto iconPath = ":/icons/texture.png";
	addNode("combinenormals", "Combine Normals", iconPath, []() {
		return new CombineNormalsNode();
	});

	addNode("texelsize", "Texel Size", iconPath, []() {
		return new TexelSizeNode();
	});

	addNode("flipbook", "Flipbook Animation", iconPath, []() {
		return new FlipbookUVAnimationNode();
	});
}

void LibraryV1::initConstants()
{

	auto iconPath = ":/icons/constant.png";
	addNode("vector2", "Vector2", iconPath, []() {
		return new Vector2Node();
	});

	addNode("vector3", "Vector3", iconPath, []() {
		return new Vector3Node();
	});

	addNode("vector4", "Vector4", iconPath, []() {
		return new Vector4Node();
	});

	// float
	addNode("float", "Float", iconPath, []()
	{
		auto floatNode = new FloatNodeModel();
		return floatNode;
	});

#if(EFFECT_BUILD_AS_LIB)
	addNode("color", "Color", iconPath, []() {
		return new ColorPickerNode();

	});
#endif
}

void LibraryV1::initUtility()
{
	auto iconPath = ":/icons/utility.png";

	// pulsate
	addNode("pulsate", "Pulsate Node", iconPath, []()
	{
		return new PulsateNode();
	});

	//make color
	addNode("makeColor", "Make Color", iconPath, []() {
		return new MakeColorNode();
	});
}
