#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include "../properties.h"

class FloatPropertyWidget : public QWidget
{
public:
    FloatPropertyWidget():
        QWidget(nullptr)
    {
        buildUi();
    }

    void buildUi()
    {
        auto layout = new QVBoxLayout();

        // title
        auto line = new QHBoxLayout();
        line->addWidget(new QLabel("Display Name"));
        displayName = new QLineEdit("Float Property");
        line->addWidget(displayName);
        layout->addLayout(line);

        line = new QHBoxLayout();
        line->addWidget(new QLabel("Value"));
        spinBox = new QDoubleSpinBox();
        line->addWidget(spinBox);
        layout->addLayout(line);

        connect(spinBox, SIGNAL(valueChanged(double)), this, SLOT(valueChanged(double)));
        connect(spinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            [=](double d){
            valueChanged(d);
        });

        this->setLayout(layout);
    }

    void setProperty(FloatProperty* prop)
    {
        this->prop = prop;
        displayName->setText(prop->displayName);
        spinBox->setValue(prop->getValue().toFloat());
    }

    QDoubleSpinBox* spinBox;
    FloatProperty* prop;
    QLineEdit* displayName;

public slots:
    void valueChanged(double newVal)
    {
        prop->setValue((float)newVal);
    }


};
