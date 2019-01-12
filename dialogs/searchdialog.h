#pragma once
#include <QDialog>
#include <QListWidget>
#include "../graph/nodegraph.h"
#include "../graph/library.h"
#include "../listwidget.h"
#include "../graphnodescene.h"


class SearchDialog : public QDialog
{
	Q_OBJECT
public:
	SearchDialog(NodeGraph *graph, GraphNodeScene* scene);
	~SearchDialog();

    ListWidget* nodeContainer;
    ListWidget* propertyContainer;
	void generateTileNode(QList<NodeLibraryItem*> lis);
	void generateTileNode(NodeGraph *graph);
	void generateTileProperty(NodeGraph *graph);
	int index = 0;

	QLineEdit *searchBar;

protected:
	void leaveEvent(QEvent *event) override;
	void showEvent(QShowEvent *event) override;

};

