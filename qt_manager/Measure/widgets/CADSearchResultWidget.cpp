#include "CADSearchResultWidget.h"
#include "ui_CADSearchResultWidget.h"
#include <QDebug>
#include "SQLCommand.h"
#include "ProductBrandStandardWidget.h"
#include "util_gui.h"

const QString productResourceTable = QStringLiteral("productResource");
const QStringList productResourcetableKeys = {
    QStringLiteral("model"), QStringLiteral("productId"), QStringLiteral("pdfPath"),
    QStringLiteral("excelPath"), QStringLiteral("updateTime")};

CADSearchResultWidget::CADSearchResultWidget(QWidget* parent)
    : QWidget(parent), ui(new Ui::CADSearchResultWidget)
{
    ui->setupUi(this);
    mmind::initUtilTableWidgetHeader(ui->tableWidget);
    hide();
}

CADSearchResultWidget::~CADSearchResultWidget() { delete ui; }

QString getSqlStr(QStringList models, QString join)
{
    QStringList joinStrs;
    for (auto iter : models)
        joinStrs.append(QString("%1='%2'").arg(join, iter));
    return QString("(%1)").arg(joinStrs.join(" or "));
}

void CADSearchResultWidget::search(QString models, QString products)
{
    _resourceInfo.clear();
    _selectedProducts.clear();
    SQLCommand sql(productResourceTable);
    QString sqlStr = QString("select %1 from %2 where %3 and %4")
                         .arg(productResourcetableKeys.join(","), productResourceTable,
                              getSqlStr(models.split(","), productResourcetableKeys[0]),
                              getSqlStr(mmind::getTranslateStr(products.split(",")),
                                        productResourcetableKeys[1]));

    auto result = sql.select(sqlStr);
    while (result->next()) {
        CADResourceInfo info{result->value(0).toString(), result->value(1).toString(),
                             result->value(2).toString(), result->value(3).toString(),
                             result->value(4).toString()};
        _resourceInfo[QString("%1_%2").arg(info.productId, info.model)] = info;
    }
    insertRow();
    show();
}

void CADSearchResultWidget::insertRow()
{
    ui->tableWidget->model()->removeRows(0, ui->tableWidget->rowCount());
    for (auto iter = _resourceInfo.begin(); iter != _resourceInfo.end(); ++iter) {
        int rowCount = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(rowCount);

        const auto checkBox =
            mmind::createCheckBoxToTableWidget(ui->tableWidget, rowCount, 0, iter.key());
        if (checkBox == nullptr)
            return;
        connect(checkBox, &QCheckBox::stateChanged, this,
                &CADSearchResultWidget::handleCheckBoxClicked);

        ui->tableWidget->setItem(rowCount, 1, new QTableWidgetItem(QString::number(rowCount + 1)));
        const auto& resourceStrs = iter.value().toStringList();
        for (int i = 0; i < resourceStrs.size(); ++i)
            ui->tableWidget->setItem(rowCount, i + 2, new QTableWidgetItem(resourceStrs[i]));
    }
}

void CADSearchResultWidget::handleCheckBoxClicked(int isSelected)
{
    auto modelAndProductStr = dynamic_cast<QCheckBox*>(sender())->objectName();
    if (isSelected == Qt::CheckState::Checked)
        _selectedProducts.insert(modelAndProductStr);
    else
        _selectedProducts.remove(modelAndProductStr);
}

void CADSearchResultWidget::on_back_pressed()
{
    emit backToPrevious();
    hide();
}

void CADSearchResultWidget::on_cad_pressed()
{
    QHash<QString, QString> pdfPath;
    for (const auto& iter : _selectedProducts) {
        auto findResourceInfoIter = _resourceInfo.find(iter);
        if (findResourceInfoIter != _resourceInfo.end())
            pdfPath[iter] = findResourceInfoIter.value().pdfPath;
    }

    QSharedPointer<ProductBrandStandardWidget> pdf(new ProductBrandStandardWidget(this));
    pdf->showPDF(pdfPath);
    pdf->exec();
}

void CADSearchResultWidget::on_measurement_pressed()
{
    QSharedPointer<ProductBrandStandardWidget> pdf(new ProductBrandStandardWidget(this));
    pdf->showMeasurementCoordinates(_selectedProducts.toList());
    pdf->exec();
}
