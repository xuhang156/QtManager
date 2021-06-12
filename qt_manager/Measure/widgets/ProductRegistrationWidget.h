#pragma once

#include <QWidget>

namespace Ui {
class ProductRegistrationWidget;
}

class ProductRegistrationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProductRegistrationWidget(QWidget* parent = nullptr);
    ~ProductRegistrationWidget();

public slots:
    void registerNewProduct();

private:
    Ui::ProductRegistrationWidget* ui;
};
