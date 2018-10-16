#include "libraryv1.h"
#include "test.h"
#include "math.h"

void LibraryV1::initTest()
{
	auto lib = this;

	// mult
	lib->addNode("vectorMultiply", "Vector Multiply", "", []()
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
}

void LibraryV1::initMath()
{
	addNode("add", "Add", "", []() {
		return new AddNode();
	});

	addNode("subtract", "Subtract", "", []() {
		return new SubtractNode();
	});

	addNode("multiply", "Multiply", "", []() {
		return new MultiplyNode();
	});

	addNode("divide", "Divide", "", []() {
		return new DivideNode();
	});

	/*
	addNode("", "", "", []() {
		return new Node();
	});
	*/

	addNode("power", "Power", "", []() {
		return new PowerNode();
	});

	addNode("sqrt", "Square Root", "", []() {
		return new SqrtNode();
	});

	addNode("min", "Min", "", []() {
		return new MinNode();
	});

	addNode("max", "Max", "", []() {
		return new MaxNode();
	});

	addNode("abs", "Abs", "", []() {
		return new AbsNode();
	});

	addNode("sign", "Sign", "", []() {
		return new SignNode();
	});

	addNode("ceil", "Ceil", "", []() {
		return new CeilNode();
	});

	addNode("floor", "Floor", "", []() {
		return new FloorNode();
	});

	addNode("round", "Round", "", []() {
		return new RoundNode();
	});

	addNode("trunc", "Truncate", "", []() {
		return new TruncNode();
	});

	addNode("step", "Step", "", []() {
		return new StepNode();
	});

	addNode("smoothstep", "Smooth Step", "", []() {
		return new SmoothStepNode();
	});

	addNode("frac", "Fraction", "", []() {
		return new FracNode();
	});

	addNode("clamp", "Clamp", "", []() {
		return new ClampNode();
	});

	addNode("lerp", "Lerp", "", []() {
		return new LerpNode();
	});

	addNode("oneminus", "One Minus", "", []() {
		return new OneMinusNode();
	});

	addNode("negate", "Negate", "", []() {
		return new NegateNode();
	});
}