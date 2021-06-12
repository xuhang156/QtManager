#include "RrSus018DReportFile.h"

#include <QDebug>
#include <QJsonArray>
#ifdef _WIN32
#include <windows.h>
#endif
#include "libxl.h"
#include "SQLCommand.h"

namespace mmind {
const QStringList tableColumnKeys = {
    QStringLiteral("pointIndex"),    QStringLiteral("type"),
    QStringLiteral("longAxis"),      QStringLiteral("shortAxis"),
    QStringLiteral("holePositionL"), QStringLiteral("holePositionw"),
    QStringLiteral("holePositionh"), QStringLiteral("surfacePosition")};
const QString taskId = QStringLiteral("taskId");
const QString xlsxTemplateFilePath = QStringLiteral("D:/018D_FrSUS.xls");

} // namespace mmind

RrSus018DData::RrSus018DData(QString tableName) : AbstractReportData(tableName) {}

bool RrSus018DData::readData()
{
    if (_resultKeys.empty())
        return false;

    SQLCommand sqlQuery(_tableName);
    auto tableDatas = sqlQuery.select(_resultKeys, _queryConditions);

    while (tableDatas->next()) {
        QStringList rowDatas;
        for (int i = 0; i < _resultKeys.size(); i++)
            rowDatas.append(tableDatas->value(i).toString());
        _resultDatas[rowDatas[0]] = rowDatas;
    }
    return false;
}

RrSus018DReportFile::RrSus018DReportFile()
{
    book->load(mmind::xlsxTemplateFilePath.toStdWString().c_str());
}

bool RrSus018DReportFile::setFromJson(const QJsonObject& jo)
{
    using namespace mmind;
    xlsxColumnIndex = jo.value(json_keys::kColumnIndex).toInt();
    if (jo.size() == 0)
        return false;

    for (const QJsonValue& jv : jo.value(json_keys::kRowIndex).toArray()) {
        auto rowIndexObj = jv.toObject();
        IndexOfMeasurementResultsToXlsx resultIndex;
        QString pointId = rowIndexObj.value(json_keys::kPointId).toString();
        resultIndex.diameter = rowIndexObj.value(json_keys::kDiameter).toInt();

        resultIndex.holeCoordinate.setFromJson(
            rowIndexObj.value(json_keys::kHoleCoordinate).toObject());
        resultIndex.ellipseAxis.setFromJson(rowIndexObj.value(json_keys::kEllipse).toObject());

        xlsxRowIndexs[pointId] = resultIndex;
    }
    return true;
}

bool RrSus018DReportFile::write(QString taskId)
{
    if (book.isNull())
        return false;

    AbstractReportData* datas = new RrSus018DData(QStringLiteral("FrSus_018D"));
    datas->setResultKeys(mmind::tableColumnKeys);
    datas->setQueryConditions(mmind::taskId, taskId);
    datas->readData();
    auto sheet = book->getSheet(0);

    const auto& dbDatas = datas->getAllDatas();
    for (const auto& iter : xlsxRowIndexs) {
        int rows = iter.second.diameter;
        auto rowDatas = dbDatas.find(iter.first);
        if (rowDatas != dbDatas.end()) {
            if (iter.second.diameter != -1) {
                for (int i = 0; i < 4; i++)
                    sheet->writeNum(iter.second.diameter + i, xlsxColumnIndex,
                                    rowDatas->at(2).toDouble());
            }
            //            if (iter.second.holeCoordinate.l != -1) {
            //                sheet->writeNum(iter.second.holeCoordinate.l, xlsxColumnIndex,
            //                                rowDatas->at(4).toDouble());
            //            }
            //            if (iter.second.holeCoordinate.w != -1) {
            //                sheet->writeNum(iter.second.holeCoordinate.w, xlsxColumnIndex,
            //                                rowDatas->at(5).toDouble());
            //            }
            //            if (iter.second.holeCoordinate.h != -1) {
            //                sheet->writeNum(iter.second.holeCoordinate.h, xlsxColumnIndex,
            //                                rowDatas->at(6).toDouble());
            //            }
            //            if (iter.second.ellipseAxis.longAxis != -1) {
            //                sheet->writeNum(iter.second.ellipseAxis.longAxis, xlsxColumnIndex,
            //                                rowDatas->at(2).toDouble());
            //            }
            //            if (iter.second.ellipseAxis.shortAxis != -1) {
            //                sheet->writeNum(iter.second.ellipseAxis.shortAxis, xlsxColumnIndex,
            //                                rowDatas->at(3).toDouble());
            //            }
        }
    }
    xlsxColumnIndex += 2;
    return true;
}
