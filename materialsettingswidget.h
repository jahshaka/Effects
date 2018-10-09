#ifndef MATERIALSETTINGSWIDGET_H
#define MATERIALSETTINGSWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>

struct MaterialSettings {
	QString name;
	Qt::CheckState zwrite;
	Qt::CheckState depthTest;
	Qt::CheckState fog;
	Qt::CheckState castShadow;
	Qt::CheckState receiveShadow;
	Qt::CheckState acceptLighting;
	int blendMode;
	int cullMode;
	int renderLayer;

};

namespace Ui {
class MaterialSettingsWidget;
}

class MaterialSettingsWidget : public QWidget
{
	Q_OBJECT

public:
	MaterialSettingsWidget(QWidget *parent = 0);
	MaterialSettingsWidget(MaterialSettings settings, QWidget *parent = 0);
	~MaterialSettingsWidget();

	void setMaterialSettings(MaterialSettings settings);

	void setName(QString name);
	void setZWrite(Qt::CheckState val);
	void setDepthText(Qt::CheckState val);
	void setFog(Qt::CheckState val);
	void setCastShadows(Qt::CheckState val);
	void setReceiveShadows(Qt::CheckState val);
	void setAcceptLighting(Qt::CheckState val);

	void setBlendMode(QString string);
	void setBlendMode(int index);
	void setCullMode(QString string);
	void setCullMode(int index);
	void setRenderLayer(QString string);
	void setRenderLayer(int index);

private:
	MaterialSettings settings;

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
