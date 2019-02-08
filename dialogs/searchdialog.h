#pragma once
#include <QDialog>
#include <QListWidget>
#include "../graph/nodegraph.h"
#include "../graph/library.h"
#include "../listwidget.h"
#include "../graphnodescene.h"
#include "../widgets//treewidget.h"


class SearchDialog : public QDialog
{
	Q_OBJECT
public:
	SearchDialog(NodeGraph *graph, GraphNodeScene* scene, QPoint point);
	~SearchDialog();

    ListWidget* propertyContainer;
	void generateTileNode(QList<NodeLibraryItem*> lis);
	void generateTileNode(NodeGraph *graph);
	void generateTileProperty(NodeGraph *graph);
	void configureTreeWidget();
	int index = 0;

	TreeWidget *tree;

	QLineEdit *searchBar;
	QPoint point;

protected:
	void leaveEvent(QEvent *event) override;
	void showEvent(QShowEvent *event) override;

};

