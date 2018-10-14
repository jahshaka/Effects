#include "texturepropertywidget.h"



TexturePropertyWidget::TexturePropertyWidget() : BasePropertyWidget()
{
//	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	auto mainLayout = layout;
	auto wid = new WidgetTexture;
	texture = wid->texture;
	setConnections();
	mainLayout->addWidget(wid);
	setLayout(mainLayout);

}


TexturePropertyWidget::~TexturePropertyWidget()
{
}

void TexturePropertyWidget::setProp(TextureProperty * prop)
{
	this->prop = prop;
	displayName->setText(prop->displayName);
	value = prop->value;
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
		QIcon icon(filename);
		texture->setIcon(icon);
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
