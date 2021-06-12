#include "ProductRegistrationWidget.h"
#include "ui_ProductRegistrationWidget.h"
#include "libs/util_gui.h"
#include "Settings/SettingsDialog.h"

ProductRegistrationWidget::ProductRegistrationWidget(QWidget* parent) : QWidget(parent) {}

ProductRegistrationWidget::~ProductRegistrationWidget() {}

void ProductRegistrationWidget::registerNewProduct()
{
    auto settings = new mmind::SettingsDialog(this);
    settings->exec();
}
