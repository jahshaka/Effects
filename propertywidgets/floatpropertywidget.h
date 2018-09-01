#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>
#include <QDoubleSpinBox>

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
        line->addWidget(new QLineEdit("Float Property"));
        layout->addLayout(line);

        line = new QHBoxLayout();
        line->addWidget(new QLabel("Value"));
        spinBox = new QDoubleSpinBox();
        line->addWidget(spinBox);
        layout->addLayout(line);

        this->setLayout(layout);
    }
    QDoubleSpinBox* spinBox;
};
