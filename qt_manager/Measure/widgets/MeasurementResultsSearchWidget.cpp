#include "MeasurementResultsSearchWidget.h"
#include "ui_MeasurementResultsSearchWidget.h"

#include <QDebug>
#include <QMessageBox>
#include "util_gui.h"
#include "util_json.h"
#include "SQLCommand.h"
#include "AbstractReportFile.h"

const int taskTableMaxColumn = 13;
const QStringList taskTableKeys{QStringLiteral("taskId"),
                                QStringLiteral("time"),
                                QStringLiteral("event"),
                                QStringLiteral("moduel"),
                                QStringLiteral("productNumber"),
                                QStringLiteral("trayNumber"),
                                QStringLiteral("numberOfQualified"),
                                QStringLiteral("numberOfMeasurements"),
                                QStringLiteral("passRate"),
                                QStringLiteral("operator"),
                                QStringLiteral("isFullTest")};

MeasurementResultsSearchWidget::MeasurementResultsSearchWidget(QWidget* parent)
    : QWidget(parent), ui(new Ui::MeasurementResultsSearchWidget)
{
    ui->setupUi(this);
    mmind::initUtilTableWidgetHeader(ui->tableWidget);
    connect(ui->tableWidget->horizontalHeader(), SIGNAL(sectionClicked(int)), ui->tableWidget,
            SLOT(sortByColumn(int)));
    hide();
}

MeasurementResultsSearchWidget::~MeasurementResultsSearchWidget() { delete ui; }

void MeasurementResultsSearchWidget::searchDBResult(const mmind::SearchInfo& info)
{
    SQLCommand query("taskInfo");
    QString sqlStr;
    if (info.measurementType == mmind::MeasureType::Product) {
        sqlStr = QString(
                     "select %1 from taskInfo where %2 in (%3) and %4 in (%5) and %6 between '%7' "
                     "and '%8'")
                     .arg(taskTableKeys.join(","), taskTableKeys[3], info.carType, taskTableKeys[4],
                          info.products, taskTableKeys[1], info.startTime, info.endTime);
    } else {
    }

    QSharedPointer<QSqlQuery> sqlQuery = query.select(sqlStr);
    while (sqlQuery->next()) {
        QStringList rowData;
        for (int i = 1; i < taskTableKeys.size(); i++)
            rowData << sqlQuery->value(taskTableKeys[i]).toString();
        insertRow(sqlQuery->value(taskTableKeys[0]).toString(), rowData);
    }
}

void MeasurementResultsSearchWidget::handleCheckBoxClicked(int isSelected)
{
    auto taskId = dynamic_cast<QCheckBox*>(sender())->objectName();
    if (isSelected == Qt::CheckState::Checked)
        _selectedTaskIds.insert(taskId);
    else
        _selectedTaskIds.remove(taskId);
}

void MeasurementResultsSearchWidget::insertRow(QString taskId, const QStringList& datas)
{
    if (datas.size() > taskTableMaxColumn)
        return;
    _taskDatas[taskId] = datas;
    int rowCount = ui->tableWidget->rowCount();

    ui->tableWidget->insertRow(rowCount);
    const auto checkBox = mmind::createCheckBoxToTableWidget(ui->tableWidget, rowCount, 0, taskId);
    if (checkBox == nullptr)
        return;
    connect(checkBox, &QCheckBox::stateChanged, this,
            &MeasurementResultsSearchWidget::handleCheckBoxClicked);

    ui->tableWidget->setItem(rowCount, 1, new QTableWidgetItem(QString::number(rowCount + 1)));
    for (int i = 0; i < datas.size(); i++)
        ui->tableWidget->setItem(rowCount, i + 2, new QTableWidgetItem(datas[i]));
}

void MeasurementResultsSearchWidget::on_report_pressed()
{
    if (ui->btnAccuracyTable->isChecked()) {
        QHash<QString, AbstractReportFile*> reportFileObjs;

        for (const auto& iter : _selectedTaskIds) {
            const auto& taskDataIter = _taskDatas.find(iter);
            if (taskDataIter != _taskDatas.end()) {
                const QString splicingNames =
                    QString("%1_%2").arg(taskDataIter.value().at(3), taskDataIter.value().at(2));

                auto findHistoryReport = reportFileObjs.find(splicingNames);
                if (findHistoryReport != reportFileObjs.end()) {
                    findHistoryReport.value()->write(iter);
                    continue;
                }

                const auto reportObj = createReportFileObj(splicingNames);
                if (reportObj == nullptr)
                    continue;
                reportObj->write(iter);
                reportFileObjs[splicingNames] = reportObj;
            }
        }
        for (auto iter : reportFileObjs)
            iter->save("D:/123.xlsx");
    }
    if (ui->btnAccuracyChart->isChecked()) {
        qDebug() << "chart";
        emit createReportChart(_selectedTaskIds);
    }
}

void MeasurementResultsSearchWidget::on_back_pressed()
{
    emit backToPrevious();
    hide();
}
