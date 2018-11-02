#include "texturepropertywidget.h"
#include "texturemanager.h"



TexturePropertyWidget::TexturePropertyWidget() : BasePropertyWidget()
{
//	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	auto mainLayout = layout;
	wid = new WidgetTexture;
	texture = wid->texture;
	setConnections();
	mainLayout->addWidget(wid);
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
	connect(texture, &QPushButton::clicked, [=]() {
		auto filename = QFileDialog::getOpenFileName();
		graphTexture->setImage(filename);

		QIcon icon(filename);
		texture->setIcon(icon);
		emit valueChanged(filename);
	});

	connect(this, &TexturePropertyWidget::valueChanged, [=](QString val) {
		setPropValue(val);
	});

	connect(this, &BasePropertyWidget::shouldSetVisible, [=](bool val) {
		wid->setVisible(val);
	});
}

void TexturePropertyWidget::setPropValue(QString value)
{
	prop->value = value;
}
