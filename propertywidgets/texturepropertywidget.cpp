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
	
#if(EFFECT_BUILD_AS_LIB)
	graphTexture = TextureManager::getSingleton()->loadTextureFromGuid(prop->value);
	QIcon icon(graphTexture->path);
	texture->setIcon(icon);
#else
	graphTexture = TextureManager::getSingleton()->createTexture();
	if (QFile::exists(prop->value))
		graphTexture->setImage(prop->value);
	QIcon icon(prop->value);
	texture->setIcon(icon);
#endif
	
	graphTexture->uniformName = prop->getUniformName();
}

QString TexturePropertyWidget::getValue()
{
	return prop->value;
}

void TexturePropertyWidget::setValue(QString guid)
{
	prop->value = guid;
}

void TexturePropertyWidget::setConnections()
{
	connect(texture, &QPushButton::clicked, [=]() {
		auto filename = QFileDialog::getOpenFileName();

#if(EFFECT_BUILD_AS_LIB)
		TextureManager::getSingleton()->removeTexture(graphTexture);
		graphTexture = TextureManager::getSingleton()->importTexture(filename);
		prop->value = graphTexture->guid;
		QIcon icon(graphTexture->path);
		texture->setIcon(icon);
#else
		graphTexture->setImage(filename);
		prop->value = filename;
		QIcon icon(filename);
		texture->setIcon(icon);
#endif

		emit valueChanged(filename, this);
	});
}

void TexturePropertyWidget::setPropValue(QString value)
{
	prop->value = value;
}
