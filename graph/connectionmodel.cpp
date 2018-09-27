#include <QUuid>
#include "connectionmodel.h"

ConnectionModel::ConnectionModel()
{
	id = QUuid::createUuid().toString();
	leftSocket = nullptr;
	rightSocket = nullptr;
}