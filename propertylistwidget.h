#ifndef PROPERTYLISTWIDGET_H
#define PROPERTYLISTWIDGET_H

#include <QWidget>

namespace Ui {
class PropertyListWidget;
}

class PropertyListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PropertyListWidget(QWidget *parent = 0);
    ~PropertyListWidget();

private:
    Ui::PropertyListWidget *ui;
};

#endif // PROPERTYLISTWIDGET_H
