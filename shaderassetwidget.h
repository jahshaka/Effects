#pragma once
#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QListWidget>
#include <QJsonObject>

#include "listwidget.h"
#include "../core/project.h"

class Database;
class ShaderAssetWidget : public QWidget
{
public:
	ShaderAssetWidget(Database *handle = Q_NULLPTR);
	~ShaderAssetWidget();


	QVBoxLayout *layout;
	QHBoxLayout *breadCrumbs;
	QListWidget *assetViewWidget;

private:
	void createCustomContextMenu(const QPoint pos);
	void updateAssetView(const QString &path, int filter = -1, bool showDependencies = false);
	void addItem(const FolderRecord &folderData);
	void addItem(const AssetRecord &assetData);
	QWidget *noWidget;

	Database *db;
	QSize currentSize = QSize(90, 90);
protected:
	bool eventFilter(QObject *watched, QEvent *event);
};

