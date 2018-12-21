#include "texturepropertywidget.h"
#include "../texturemanager.h"



TexturePropertyWidget::TexturePropertyWidget() : BasePropertyWidget()
{
	auto wid = new QWidget;
	auto winHolder = new QVBoxLayout;
	winHolder->setContentsMargins(0, 0, 0, 0);
	wid->setLayout(winHolder);

	auto label = new QLabel("Value :");
	texture = new QPushButton();
	texture->setIconSize(QSize(145, 145));
	texture->setMinimumSize(160, 146);
	winHolder->addWidget(label);
	winHolder->addWidget(texture);


	//layout->addWidget(wid);

	setStyleSheet("background:rgba(0,0,0,0); color: rgba(250,250,250,.9);");
	texture->setStyleSheet("background:rgba(0,0,0,0); border : 2px solid rgba(50,50,50,.3);");
	setWidget(wid);
	setConnections();
}


TexturePropertyWidget::~TexturePropertyWidget()
{
}

void TexturePropertyWidget::setProp(TextureProperty * prop)
{
	this->prop = prop;
	displayName->setText(prop->displayName);
	value = prop->value;
	modelProperty = prop;

	graphTexture = TextureManager::getSingleton()->createTexture();
	graphTexture->uniformName = prop->getUniformName();
	if (QFile::exists(value))
		graphTexture->setImage(value);

	//emit nameChanged(displayName->text());
	//emit valueChanged(value, this);
}

QString TexturePropertyWidget::getValue()
{
	return value;
}

void TexturePropertyWidget::setValue(QString guid)
{
	prop->setValue(guid);
	value = guid;
}

void TexturePropertyWidget::setConnections()
{
	connect(texture, &QPushButton::clicked, [=]() {
		auto filename = QFileDialog::getOpenFileName();
		graphTexture->setImage(filename);

		QIcon icon(filename);
		texture->setIcon(icon);
		emit valueChanged(filename, this);
	});

	connect(this, &TexturePropertyWidget::valueChanged, [=](QString val) {
		setPropValue(val);
	});
}

void TexturePropertyWidget::setPropValue(QString value)
{
	prop->value = value;
}
