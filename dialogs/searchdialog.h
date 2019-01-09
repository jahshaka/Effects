#pragma once
#include <QDialog>
#include <QListWidget>
#include "../graph/nodegraph.h"
#include "../graph/library.h"
#include "../listwidget.h"


class SearchDialog : public QDialog
{
	Q_OBJECT
public:
	SearchDialog(NodeGraph *graph);
	~SearchDialog();

	QListWidget* nodeContainer;
	QListWidget* propertyContainer;
	void generateTileNode(QList<NodeLibraryItem*> lis);
	void generateTileNode(NodeGraph *graph);
	void generateTileProperty(NodeGraph *graph);
	int index = 0;

protected:
	void focusOutEvent(QFocusEvent *event) override;
	void leaveEvent(QEvent *event) override;

};

