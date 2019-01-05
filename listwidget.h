#pragma once
#include <QListWidget>
#include <QSize>

struct shaderInfo {
	QString GUID;
	QString name;
};

class ListWidget : public QListWidget
{

    Q_OBJECT
public:
	ListWidget();
	~ListWidget();

	void displayAllContents();
	bool isResizable = false;
	void dropEvent(QDropEvent *event) override;
    bool shaderContextMenuAllowed = false;
    void createContextMenu();

	QSize itemSize;
	int numberOfItemPerRow;
	void addToListWidget(QListWidgetItem *item);

private slots:
    void customContextMenu(QPoint pos);
	

protected:
    QMimeData * mimeData(const QList<QListWidgetItem *> items) const override;
    void resizeEvent(QResizeEvent * event) override;

signals:
    void renameShader(QString guid);
    void exportShader(QString guid);
    void editShader(QString guid);
    void deleteShader(QString guid);
    void createShader(QString guid);
    void importShader(QString guid);

	void resizeItem(int size);
};

