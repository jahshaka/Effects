#include "graphtest.h"

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

    // time
    graph->registerModel("Time", []()
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
    graph->registerModel("Sine", []()
    {
        return new SineNode();
    });


}
