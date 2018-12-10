#pragma once
#include <QListWidget>


struct shaderInfo {
	QString GUID;
	QString name;
};

class ListWidget : public QListWidget
{
public:
	ListWidget();
	~ListWidget();

	void displayAllContents();
	bool isResizable = false;
	void dropEvent(QDropEvent *event) override;

protected:
	QMimeData * mimeData(const QList<QListWidgetItem *> items) const;
	void resizeEvent(QResizeEvent * event);
};

