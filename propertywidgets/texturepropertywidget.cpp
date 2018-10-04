#include "texturepropertywidget.h"



TexturePropertyWidget::TexturePropertyWidget()
{
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	auto mainLayout = new QVBoxLayout();
	auto textLayout = new QHBoxLayout();

	displayName = new QLineEdit(this);
	auto label = new QLabel("Display Name", this);

	textLayout->addWidget(label);
	textLayout->addStretch();
	textLayout->addWidget(displayName);
	auto wid = getValueWidget();

	mainLayout->addLayout(textLayout);
	mainLayout->addWidget(wid);
	setLayout(mainLayout);

	widget->setStyleSheet("background:rgba(0,0,0,0); color: rgba(250,250,250,.9);");
	texture->setStyleSheet("background:rgba(0,0,0,0); border : 2px solid rgba(50,50,50,.3);");

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

QWidget * TexturePropertyWidget::getValueWidget()
{
	widget = new QWidget(this);
	auto mainLayout = new QVBoxLayout;
	
	auto label = new QLabel("Value :", this);

	widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	widget->setLayout(mainLayout);

	texture = new QPushButton();
	texture->setIconSize(QSize(145, 145));
	texture->setMinimumSize(160, 146);
	mainLayout->addWidget(label);
	mainLayout->addWidget(texture);

	connect(texture, &QPushButton::clicked, [=]() {
		auto filename = QFileDialog::getOpenFileName();
		QIcon icon(filename);
		texture->setIcon(icon);
		emit valueChanged(filename);
	});

	widget->setStyleSheet("background:rgba(0,0,0,0); color: rgba(250,250,250,.9);");
	texture->setStyleSheet("background:rgba(0,0,0,0); border : 2px solid rgba(50,50,50,.3);");

	connect(this, &TexturePropertyWidget::valueChanged, [=](QString val) {
		setPropValue(val);
	});
	return widget;
}

QWidget * TexturePropertyWidget::getWidget()
{
	if (widget) return widget;
	return nullptr;
}

void TexturePropertyWidget::paintEvent(QPaintEvent * event)
{
	QWidget::paintEvent(event);
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(QPen(QColor(200, 200, 200, 70), 2));
	painter.drawRect(0, 0, width(), height());
}

void TexturePropertyWidget::setPropValue(QString value)
{
	prop->value = value;
}
