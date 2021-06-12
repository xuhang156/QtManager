#pragma once
#include "AbstractReportFile.h"

#include <QObject>
#include "json_keys.h"

namespace mmind {
struct HoleCoordinate
{
    int l;
    int w;
    int h;
    bool setFromJson(const QJsonObject& jo)
    {
        l = jo.value(json_keys::kLAxis).toInt(-1);
        w = jo.value(json_keys::kWAxis).toInt(-1);
        h = jo.value(json_keys::kHAxis).toInt(-1);
        return true;
    }
};

struct EllipseLongAndShortAxis
{
    int longAxis;
    int shortAxis;
    bool setFromJson(const QJsonObject& jo)
    {
        longAxis = jo.value(json_keys::kLongAxis).toInt(-1);
        shortAxis = jo.value(json_keys::kShortAxis).toInt(-1);
        return true;
    }
};

struct IndexOfMeasurementResultsToXlsx
{
    int diameter;
    HoleCoordinate holeCoordinate;
    EllipseLongAndShortAxis ellipseAxis;
};
} // namespace mmind

class RrSus018DReportFile : public AbstractReportFile
{
public:
    RrSus018DReportFile();
    virtual bool write(QString taskId);
    virtual bool setFromJson(const QJsonObject& jo);

private:
    // std::map<int, mmind::IndexOfMeasurementResultsToXlsx> xlsxRowIndex;
    std::map<QString, mmind::IndexOfMeasurementResultsToXlsx> xlsxRowIndexs;
    int xlsxColumnIndex;
};

class RrSus018DData : public AbstractReportData
{
public:
    RrSus018DData(QString tableName);
    void read() { readData(); }
    bool readData();
};
