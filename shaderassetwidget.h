#pragma once
#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QListWidget>
#include <QJsonObject>

#include "listwidget.h"
#include "shaderlistwidget.h"
#include "../core/project.h"


//#include "../widgets/assetwidget.h"

struct AssetItemShader {
	QString selectedPath; 
	QListWidgetItem *wItem;
	QString selectedGuid;
	// add one for assetView maybe...
};



class Database;
class ShaderAssetWidget : public QWidget
{
public:
	ShaderAssetWidget(Database *handle = Q_NULLPTR);
	~ShaderAssetWidget();


	QVBoxLayout *layout;
	QHBoxLayout *breadCrumbs;
	
	AssetItemShader assetItemShader;
	

	void createCustomContextMenu(const QPoint pos);
	void addItem(const FolderRecord &folderData);
	void addItem(const AssetRecord &assetData);
	void createFolder();
	void createShader(QString *shaderName = Q_NULLPTR);
	void createShader(QListWidgetItem *item);
	void createLocalShader();

#if(EFFECT_BUILD_AS_LIB)
	ShaderListWidget *assetViewWidget;

	void updateAssetView(const QString &path, int filter = -1, bool showDependencies = false);
	void setUpDatabse(Database *db);
	void refresh();
#endif
private:

	QWidget *noWidget;

	Database *db;
	QSize currentSize = QSize(90, 90);
protected:
	bool eventFilter(QObject *watched, QEvent *event);
};

