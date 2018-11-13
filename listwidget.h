#pragma once
#include <QListWidget>


class ListWidget : public QListWidget
{
public:
	ListWidget();
	~ListWidget();

protected:
	QMimeData * mimeData(const QList<QListWidgetItem *> items) const;
};

