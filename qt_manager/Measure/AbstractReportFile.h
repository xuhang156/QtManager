#pragma once
#include <QHash>
#include <QVector>
#include <QSharedPointer>
#include <QJsonObject>
#include "libxl.h"

class AbstractReportFile
{
public:
    AbstractReportFile() : book(xlCreateBook()) {}
    virtual ~AbstractReportFile() = default;
    virtual bool save(QString filePath, bool releaseBook = false);
    virtual bool write(QString taskId) = 0;
    bool init(QString productAndModelName);

private:
    virtual bool setFromJson(const QJsonObject& jo) = 0;

protected:
    QSharedPointer<libxl::Book> book;
};

class AbstractReportChart
{
public:
    AbstractReportChart() {}
    virtual ~AbstractReportChart() = default;
    virtual bool createChart(QString taskId) = 0;

protected:
};

class AbstractReportData
{
public:
    AbstractReportData(QString tableName) : _tableName(tableName) {}
    virtual ~AbstractReportData() = default;

    virtual void setResultKeys(const QStringList& keys) { _resultKeys = keys; }
    virtual void setQueryConditions(QString key, QString value) { _queryConditions[key] = value; }
    virtual QHash<QString, QStringList> getAllDatas() { return _resultDatas; }
    virtual bool readData() = 0;

protected:
    QString _tableName;
    QStringList _resultKeys;
    QHash<QString, QStringList> _resultDatas;
    QHash<QString, QString> _queryConditions;
};

AbstractReportFile* createReportFileObj(QString productAndModelName);
