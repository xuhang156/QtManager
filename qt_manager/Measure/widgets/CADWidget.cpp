#include <QDebug>
#include "util_gui.h"
#include "CADWidget.h"
#include "ui_CadWidget.h"
#include "SettingsDialog.h"

CADWidget::CADWidget(QWidget* parent) : QWidget(parent), ui(new Ui::CADWidget)
{
    ui->setupUi(this);

    utilComBoxGroups = mmind::addUtilComboxGroupsItem(ui->formLayout);
    ui->gridLayout->setAlignment(ui->widget, Qt::AlignCenter);
}

CADWidget::~CADWidget() { delete ui; }

void CADWidget::on_search_pressed()
{
    if (utilComBoxGroups.isNull())
        return;
    emit searchBrandStandard(utilComBoxGroups->carType->currentText(),
                             utilComBoxGroups->product->currentText());
    hide();
}

void CADWidget::on_regist_pressed()
{
    auto settings = new mmind::SettingsDialog(this);
    settings->exec();
}
