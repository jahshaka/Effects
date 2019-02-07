#include "library.h"
#include "nodemodel.h"

QVector<NodeLibraryItem*> NodeLibrary::getItems()
{
	return items;
}

QVector<NodeLibraryItem*> NodeLibrary::filter(QString name)
{
	QVector<NodeLibraryItem*> filtered;

	for (auto item : items) {
		if (item->displayName.toLower().contains(name))
			filtered.append(item);
	}

	return filtered;
}

void NodeLibrary::addNode(QString name, QString displayName, QIcon icon, std::function<NodeModel *()> factoryFunction)
{
	items.append(new NodeLibraryItem{ name, displayName, icon , factoryFunction });
}

void NodeLibrary::addNode(QString name, QString displayName, QString iconPath, std::function<NodeModel *()> factoryFunction)
{
	items.append(new NodeLibraryItem{ name, displayName, QIcon(iconPath) , factoryFunction });
}

bool NodeLibrary::hasNode(QString name)
{
	for (auto item : items)
		if (item->name == name)
			return true;
	return false;
}

NodeModel* NodeLibrary::createNode(QString name)
{
	for (auto item : items)
		if (item->name == name)
			return item->factoryFunction();
	return nullptr;
}

NodeLibrary::NodeLibrary()
{

}

NodeLibrary::~NodeLibrary()
{

}