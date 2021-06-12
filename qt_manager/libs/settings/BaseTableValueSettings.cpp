#include "BaseTableValueSettings.h"
#include "ui_BaseTableValueSettings.h"

#include <QDebug>
//#include "libs/util_gui.h"

namespace mmind {

BaseTableValueSettins::BaseTableValueSettins(QWidget* parent)
    : QWidget(parent), ui(new Ui::BaseTableValueSettins)
{
    ui->setupUi(this);
}

BaseTableValueSettins::~BaseTableValueSettins() { delete ui; }

void BaseTableValueSettins::initWidget(QString labelName, QStringList tableHeaders)
{
    ui->label->setText(labelName);
    ui->tableWidget->setColumnCount(tableHeaders.size());
    ui->tableWidget->setVerticalHeaderLabels(tableHeaders);
    ui->tableWidget->setHorizontalHeaderLabels(tableHeaders);
    // initUtilTableWidgetHeader(ui->tableWidget);
}

void BaseTableValueSettins::insertRow()
{
    const int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);
    QTableWidgetItem* item = new QTableWidgetItem();
    //    auto combox = new QComboBox();
    //    combox->addItems({"longAxis", "shortAxis"});
    //    combox->setStyleSheet("QComboBox{margin:3px};");
    //    ui->tableWidget->setCellWidget(row, 1, combox);
}

void BaseTableValueSettins::on_addItem_pressed() { insertRow(); }

void BaseTableValueSettins::on_removeItem_pressed()
{
    ui->tableWidget->removeRow(ui->tableWidget->currentRow());
}

} // namespace mmind
