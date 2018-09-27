#include <QString>
#include <QList>

#include "shadercontext.h"
#include "../graph/nodemodel.h"
#include "../graph/socketmodel.h"

// created a temporary variable using the socket
TempVar ShaderContext::createTempVar(SocketModel* sock)
{
	if (tempVars.contains(sock->id))
		return tempVars[sock->id];

	TempVar tempVar;
	tempVar.name = QString("temp%1").arg(tempVars.count());
	tempVar.typeName = sock->typeName;
	tempVars.insert(sock->id, tempVar);

	return tempVar;
}

QList<TempVar> ShaderContext::getTempVars()
{
	return tempVars.values();
}

void ShaderContext::clear()
{
	tempVars.clear();
}

void ShaderContext::addCodeChunk(NodeModel* node, QString code)
{
	QString ownerName = node->title + " - " + node->id;
	//CodeChunk chunk = {node, code};
	codeChunks.append({ ownerName, code });
}

// add declaration for uniform here
void ShaderContext::addUniform(QString uniformDecl)
{
	uniforms.append(uniformDecl);
}

QString ShaderContext::generateVars()
{
	QString finalCode = "";

	// generate temp vars
	for (auto& var : tempVars) {
		auto c = var.typeName + " " + var.name + ";\n";
		finalCode = c + finalCode;
	}

	return finalCode;
}

// generates uniforms from properties
QString ShaderContext::generateUniforms()
{
	QString finalCode = "";

	// generate temp vars
	for (auto& uniform : uniforms) {
		finalCode += uniform + ";\n";
	}

	return finalCode;
}

QString ShaderContext::generateCode(bool debug)
{
	QString finalCode = "";

	// combine code chunks
	for (auto chunk : codeChunks) {
		if (debug)
			finalCode += "// " + chunk.owner + "\n";

		finalCode += chunk.code;
		finalCode += "\n";

		// extra space for debug mode
		if (debug)
			finalCode += "\n";
	}

	return finalCode;
}