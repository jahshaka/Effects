#include <QLineEdit>
#include <QComboBox>
#include <QDoubleValidator>
#include <QVBoxLayout>

#include "../graph/nodegraph.h"
#include "../graph/nodemodel.h"
#include "../graph/sockets.h"
#include "../generator/shadercontext.h"

class SurfaceMasterNode : public NodeModel
{
public:
	SurfaceMasterNode();
	virtual void process(ModelContext* ctx) override;
};


class FloatNodeModel : public NodeModel
{
	QLineEdit* lineEdit;

	FloatSocketModel* valueSock;
public:
	FloatNodeModel();

	void editTextChanged(const QString& text);
	virtual void process(ModelContext* context) override;

	virtual QJsonValue serializeWidgetValue(int widgetIndex = 0) override;

	virtual void deserializeWidgetValue(QJsonValue val, int widgetIndex = 0) override;
};

class VectorMultiplyNode : public NodeModel
{
public:
	VectorMultiplyNode();

	virtual void process(ModelContext* context) override;
};

class WorldNormalNode : public NodeModel
{
public:
	WorldNormalNode();

	virtual void process(ModelContext* context) override;
};

class TimeNode : public NodeModel
{
public:
	TimeNode();

	virtual void process(ModelContext* context) override;
};

class SineNode : public NodeModel
{
public:
	SineNode();

	virtual void process(ModelContext* context) override;
};

class MakeColorNode : public NodeModel
{
public:
	MakeColorNode();

	virtual void process(ModelContext *context) override;
};

class TextureCoordinateNode : public NodeModel
{
	QComboBox* combo;
	QString uv;
public:
	TextureCoordinateNode();

	virtual void process(ModelContext* context) override;

	void comboTextChanged(const QString& text);
};

class PropertyNode : public NodeModel
{
	Property* prop;
public:
	PropertyNode();

	// doesnt own property
	void setProperty(Property* property);

	virtual QJsonValue serializeWidgetValue(int widgetIndex = 0) override;

	virtual void process(ModelContext* context) override;
};

void registerModels(NodeGraph* graph);