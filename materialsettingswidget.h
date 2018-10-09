#ifndef MATERIALSETTINGSWIDGET_H
#define MATERIALSETTINGSWIDGET_H

#include <QWidget>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include "graph/nodegraph.h"

namespace Ui {
class MaterialSettingsWidget;
}
class MaterialSettingsWidget : public QWidget
{
	Q_OBJECT

public:
	MaterialSettingsWidget(QWidget *parent = 0);
	MaterialSettingsWidget(MaterialSettings* settings, QWidget *parent = 0);
	~MaterialSettingsWidget();

	void setMaterialSettings(MaterialSettings* settings);

	void setName(QString name);
	void setZWrite(bool val);
	void setDepthText(bool val);
	void setFog(bool val);
	void setCastShadows(bool val);
	void setReceiveShadows(bool val);
	void setAcceptLighting(bool val);

	void setBlendMode(QString string);
	void setBlendMode(int index);
	void setCullMode(QString string);
	void setCullMode(int index);
	void setRenderLayer(QString string);
	void setRenderLayer(int index);

private:
	MaterialSettings* settings;

	void setConnections();
	Ui::MaterialSettingsWidget* ui;
	QGridLayout *gridLayout;
	QFormLayout *formLayout;
	QLabel *label;
	QLineEdit *lineEdit;
	QLabel *label_2;
	QCheckBox *checkBox;
	QLabel *label_3;
	QCheckBox *checkBox_2;
	QLabel *label_4;
	QComboBox *comboBox;
	QLabel *label_5;
	QComboBox *comboBox_2;
	QLabel *label_6;
	QComboBox *comboBox_3;
	QLabel *label_7;
	QCheckBox *checkBox_3;
	QLabel *label_8;
	QLabel *label_9;
	QLabel *label_10;
	QCheckBox *checkBox_4;
	QCheckBox *checkBox_5;
	QCheckBox *checkBox_6;

signals:
	void settingsChanged(MaterialSettings value);
};

#endif
