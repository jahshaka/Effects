#pragma once
#include <QDialog>
#include <QListWidget>
#include "../graph/nodegraph.h"
#include "../graph/library.h"
#include "../listwidget.h"


class SearchDialog : public QDialog
{
public:
	SearchDialog(NodeGraph *graph);
	~SearchDialog();

	QListWidget* nodeContainer;
	void generateTileNode(QList<NodeLibraryItem*> lis);
	void generateTileNode(NodeGraph *graph);

};

