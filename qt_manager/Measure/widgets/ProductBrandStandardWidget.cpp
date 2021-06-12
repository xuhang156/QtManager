#include "ProductBrandStandardWidget.h"
#include "ui_ProductBrandStandardWidget.h"

#include <QFileInfo>
#include <QDebug>
#include <QMessageBox>
#include "util_gui.h"
#include "qpdfwidget.h"
#include "SQLCommand.h"

const int tableMaxCol = 6;
const QString tableName = QStringLiteral("productBrandStandard");

ProductBrandStandardWidget::ProductBrandStandardWidget(QWidget* parent)
    : QDialog(parent), ui(new Ui::ProductBrandStandardWidget)
{
    ui->setupUi(this);
    mmind::initUtilTableWidgetHeader(ui->tableWidget);
    showPDF(true);
}

ProductBrandStandardWidget::~ProductBrandStandardWidget() { delete ui; }

void ProductBrandStandardWidget::showPDF(bool isPDF)
{
    ui->tabWidget->setVisible(isPDF);
    ui->tableWidget->setVisible(!isPDF);
}

void ProductBrandStandardWidget::showMeasurementCoordinates(const QStringList& pdfPaths)
{
    showPDF(false);
    for (auto iter : pdfPaths) {
        SQLCommand sql(tableName);
        auto results = sql.select({}, {{QStringLiteral("id"), iter}}, true);

        while (results->next()) {
            int count = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(count);
            for (int i = 0; i < tableMaxCol; i++)
                ui->tableWidget->setItem(count, i,
                                         new QTableWidgetItem(results->value(i).toString()));
        }
    }
}

void ProductBrandStandardWidget::showPDF(const QHash<QString, QString>& pdfPaths)
{
    showPDF(true);
    QStringList errorLoad;
    for (auto iter = pdfPaths.begin(); iter != pdfPaths.end(); ++iter) {
        QFileInfo fileInfo(iter.value());
        if (!fileInfo.isFile()) {
            errorLoad.append(iter.key());
            continue;
        }
        QPdfWidget* pdfWidget = new QPdfWidget(this);
        pdfWidget->loadFile(iter.value());
        ui->tabWidget->addTab(pdfWidget, iter.key());
    }
    if (!errorLoad.empty())
        QMessageBox::critical(this, "Title", QString("Open failed: %1").arg(errorLoad.join(";")),
                              QMessageBox::Yes);
}
