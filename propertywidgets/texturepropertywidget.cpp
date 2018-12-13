#include "texturepropertywidget.h"
#include "../texturemanager.h"



TexturePropertyWidget::TexturePropertyWidget() : BasePropertyWidget()
{
	wid = new WidgetTexture;
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

	emit nameChanged(displayName->text());
	emit valueChanged(value);
}

QString TexturePropertyWidget::getValue()
{
	return value;
}

void TexturePropertyWidget::setConnections()
{
	connect(wid->texture, &QPushButton::clicked, [=]() {
		auto filename = QFileDialog::getOpenFileName();
		graphTexture->setImage(filename);

		QIcon icon(filename);
		wid->texture->setIcon(icon);
		emit valueChanged(filename);
	});

	connect(this, &TexturePropertyWidget::valueChanged, [=](QString val) {
		setPropValue(val);
	});
}

void TexturePropertyWidget::setPropValue(QString value)
{
	prop->value = value;
}
