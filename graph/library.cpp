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

void NodeLibrary::addNode(QString name, QString displayName, QIcon icon, NodeCategory type, std::function<NodeModel *()> factoryFunction)
{
	items.append(new NodeLibraryItem{ name, displayName, icon , type, factoryFunction });
}

void NodeLibrary::addNode(QString name, QString displayName, QString iconPath, NodeCategory type, std::function<NodeModel *()> factoryFunction)
{
	items.append(new NodeLibraryItem{ name, displayName, QIcon(iconPath) ,type, factoryFunction });
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
		if (item->name == name) {
			auto node = item->factoryFunction();
			node->setNodeType(item->nodeCategory);
			return node;
		}
	return nullptr;
}

NodeLibrary::NodeLibrary()
{

}

NodeLibrary::~NodeLibrary()
{

}