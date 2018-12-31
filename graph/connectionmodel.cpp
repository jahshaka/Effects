#include <QUuid>
#include "connectionmodel.h"
#include "../core/guidhelper.h"

ConnectionModel::ConnectionModel()
{
	id = GuidHelper::createGuid();
	leftSocket = nullptr;
	rightSocket = nullptr;
}