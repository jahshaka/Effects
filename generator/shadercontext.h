#ifndef SHADER_CONTEXT_H
#define SHADER_CONTEXT_H

#include <QString>
#include <QList>
#include <QMap>

#include "../graph/nodegraph.h"

class SocketModel;

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
	QStringList uniforms;
	// mapped using the socket guid
	QMap<QString, TempVar> tempVars;
	QList<CodeChunk> codeChunks;
	//QString code;
	bool debugCode;
public:
	void addFunction(QString name, QString function);

	// created a temporary variable using the socket
	TempVar createTempVar(SocketModel* sock);

	QList<TempVar> getTempVars();
	void clear();

	void addCodeChunk(NodeModel* node, QString code);

	// add declaration for uniform here
	void addUniform(QString uniformDecl);
	QString generateVars();

	// generates uniforms from properties
	QString generateUniforms();

	QString generateCode(bool debug = false);
};

#endif// SHADER_CONTEXT_H