#include "materialsettingswidget.h"
#include "ui_materialsettingswidget.h"


MaterialSettingsWidget::MaterialSettingsWidget(QWidget *parent):
	QWidget(parent),
	ui(new Ui::MaterialSettingsWidget)
{
	//ui->setupUi(this);

	if (this->objectName().isEmpty())
		this->setObjectName(QStringLiteral("MaterialSettingsWidget"));

	this->resize(330, 433);
	gridLayout = new QGridLayout(this);
	gridLayout->setObjectName(QStringLiteral("gridLayout"));
	gridLayout->setHorizontalSpacing(0);
	gridLayout->setContentsMargins(9, 3, 9, 0);
	formLayout = new QFormLayout();
	formLayout->setObjectName(QStringLiteral("formLayout"));
	formLayout->setContentsMargins(5, 5, 5, 5);
	label = new QLabel("Name",this);
	label->setObjectName(QStringLiteral("label"));
	formLayout->setWidget(0, QFormLayout::LabelRole, label);
	lineEdit = new QLineEdit(this);
	lineEdit->setObjectName(QStringLiteral("lineEdit"));
	formLayout->setWidget(0, QFormLayout::FieldRole, lineEdit);
	label_2 = new QLabel("Z Write",this);
	label_2->setObjectName(QStringLiteral("label_2"));
	formLayout->setWidget(1, QFormLayout::LabelRole, label_2);
	checkBox = new QCheckBox(this);
	checkBox->setObjectName(QStringLiteral("checkBox"));
	checkBox->setLayoutDirection(Qt::RightToLeft);
	formLayout->setWidget(1, QFormLayout::FieldRole, checkBox);
	label_3 = new QLabel("Depth Test",this);
	label_3->setObjectName(QStringLiteral("label_3"));
	formLayout->setWidget(2, QFormLayout::LabelRole, label_3);
	checkBox_2 = new QCheckBox(this);
	checkBox_2->setObjectName(QStringLiteral("checkBox_2"));
	checkBox_2->setLayoutDirection(Qt::RightToLeft);
	formLayout->setWidget(2, QFormLayout::FieldRole, checkBox_2);
	label_4 = new QLabel("Blend Mode",this);
	label_4->setObjectName(QStringLiteral("label_4"));
	formLayout->setWidget(3, QFormLayout::LabelRole, label_4);
	comboBox = new QComboBox(this);
	comboBox->setObjectName(QStringLiteral("comboBox"));
	formLayout->setWidget(3, QFormLayout::FieldRole, comboBox);
	label_5 = new QLabel("Cul Mode", this);
	label_5->setObjectName(QStringLiteral("label_5"));
	formLayout->setWidget(4, QFormLayout::LabelRole, label_5);
	comboBox_2 = new QComboBox(this);
	comboBox_2->setObjectName(QStringLiteral("comboBox_2"));
	formLayout->setWidget(4, QFormLayout::FieldRole, comboBox_2);
	label_6 = new QLabel("Render Layer", this);
	label_6->setObjectName(QStringLiteral("label_6"));
	formLayout->setWidget(5, QFormLayout::LabelRole, label_6);
	comboBox_3 = new QComboBox(this);
	comboBox_3->setObjectName(QStringLiteral("comboBox_3"));
	formLayout->setWidget(5, QFormLayout::FieldRole, comboBox_3);
	label_7 = new QLabel("Fog", this);
	label_7->setObjectName(QStringLiteral("label_7"));
	formLayout->setWidget(6, QFormLayout::LabelRole, label_7);
	checkBox_3 = new QCheckBox(this);
	checkBox_3->setObjectName(QStringLiteral("checkBox_3"));
	checkBox_3->setLayoutDirection(Qt::RightToLeft);
	formLayout->setWidget(6, QFormLayout::FieldRole, checkBox_3);
	label_8 = new QLabel("Cast shadows" , this);
	label_8->setObjectName(QStringLiteral("label_8"));
	formLayout->setWidget(7, QFormLayout::LabelRole, label_8);
	label_9 = new QLabel("Receive shadows",this);
	label_9->setObjectName(QStringLiteral("label_9"));
	formLayout->setWidget(8, QFormLayout::LabelRole, label_9);
	label_10 = new QLabel("Accept Light", this);
	label_10->setObjectName(QStringLiteral("label_10"));
	formLayout->setWidget(9, QFormLayout::LabelRole, label_10);
	checkBox_4 = new QCheckBox(this);
	checkBox_4->setObjectName(QStringLiteral("checkBox_4"));
	checkBox_4->setLayoutDirection(Qt::RightToLeft);
	formLayout->setWidget(7, QFormLayout::FieldRole, checkBox_4);
	checkBox_5 = new QCheckBox(this);
	checkBox_5->setObjectName(QStringLiteral("checkBox_5"));
	checkBox_5->setLayoutDirection(Qt::RightToLeft);
	formLayout->setWidget(8, QFormLayout::FieldRole, checkBox_5);
	checkBox_6 = new QCheckBox(this);
	checkBox_6->setObjectName(QStringLiteral("checkBox_6"));
	checkBox_6->setLayoutDirection(Qt::RightToLeft);
	formLayout->setWidget(9, QFormLayout::FieldRole, checkBox_6);
	gridLayout->addLayout(formLayout, 0, 0, 1, 1);
	formLayout->setSpacing(5);
	setLayout(gridLayout);

	setConnections();
}

MaterialSettingsWidget::~MaterialSettingsWidget()
{
	delete ui;
}

void MaterialSettingsWidget::setName(QString name)
{
	lineEdit->setText(name);
}

void MaterialSettingsWidget::setZWrite(Qt::CheckState val)
{
	checkBox->setCheckState(val);
}

void MaterialSettingsWidget::setDepthText(Qt::CheckState val)
{
	checkBox_2->setCheckState(val);
}

void MaterialSettingsWidget::setFog(Qt::CheckState val)
{
	checkBox_3->setCheckState(val);
}

void MaterialSettingsWidget::setCastShadows(Qt::CheckState val)
{
	checkBox_4->setCheckState(val);
}

void MaterialSettingsWidget::setReceiveShadows(Qt::CheckState val)
{
	checkBox_5->setCheckState(val);
}

void MaterialSettingsWidget::setAcceptLighting(Qt::CheckState val)
{
	checkBox_6->setCheckState(val);
}

void MaterialSettingsWidget::setBlendMode(QString string)
{
	comboBox->setCurrentText(string);
}

void MaterialSettingsWidget::setBlendMode(int index)
{
	comboBox->setCurrentIndex(index);
}

void MaterialSettingsWidget::setCullMode(QString string)
{
	comboBox_2->setCurrentText(string);
}

void MaterialSettingsWidget::setCullMode(int index)
{
	comboBox_2->setCurrentIndex(index);
}

void MaterialSettingsWidget::setRenderLayer(QString string)
{
	comboBox_3->setCurrentText(string);
}

void MaterialSettingsWidget::setRenderLayer(int index)
{
	comboBox_3->setCurrentIndex(index);
}

void MaterialSettingsWidget::setConnections()
{
	connect(checkBox, &QCheckBox::stateChanged, [](int state) {}); // Z Write
	connect(checkBox_2, &QCheckBox::stateChanged, [](int state) {}); // Depth Test
	connect(checkBox_3, &QCheckBox::stateChanged, [](int state) {}); // Fog
	connect(checkBox_4, &QCheckBox::stateChanged, [](int state) {}); // Cast Shadow
	connect(checkBox_5, &QCheckBox::stateChanged, [](int state) {}); // Recieve shadows
	connect(checkBox_6, &QCheckBox::stateChanged, [](int state) {}); // Accept light

	connect(lineEdit, &QLineEdit::textChanged, [](QString string) {}); // Name
	connect(comboBox, QOverload<int>::of( &QComboBox::currentIndexChanged), [](int index) {}); //blendmode
	connect(comboBox, QOverload<const QString &>::of( &QComboBox::currentIndexChanged), [](QString text) {}); // blendmode
	connect(comboBox_2, QOverload<int>::of( &QComboBox::currentIndexChanged), [](int index) {}); // cull mode
	connect(comboBox_2, QOverload<const QString &>::of( &QComboBox::currentIndexChanged), [](QString text) {}); //cull mode
	connect(comboBox_3, QOverload<int>::of( &QComboBox::currentIndexChanged), [](int index) {}); // render layer
	connect(comboBox_3, QOverload<const QString &>::of( &QComboBox::currentIndexChanged), [](QString text) {}); //rendr Layer
}
