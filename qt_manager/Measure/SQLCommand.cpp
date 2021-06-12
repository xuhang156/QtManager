#include <QDebug>
#include "SQLCommand.h"

QSharedPointer<QSqlQuery> exec(QString queryStr)
{
    QSharedPointer<QSqlQuery> sqlQuery(new QSqlQuery);
    sqlQuery->exec(queryStr);
    return sqlQuery;
}

QSharedPointer<QSqlQuery> SQLCommand::select(QStringList resultKeys,
                                             QHash<QString, QString> queryConditions,
                                             bool selectAll)
{
    QString queryStr = QString("select %2 from %1 where ").arg(_tableName);
    selectAll ? queryStr = queryStr.arg("*") : queryStr = queryStr.arg(resultKeys.join(","));

    auto iter = queryConditions.begin();
    while (iter != queryConditions.end()) {
        queryStr += QString("%1 = '%2' and ").arg(iter.key(), iter.value());
        ++iter;
    }
    queryStr.replace(queryStr.lastIndexOf("and"), 3, ';');
    qDebug() << queryStr;
    return exec(queryStr);
}

QSharedPointer<QSqlQuery> SQLCommand::select(QString sqlStr)
{
    if (sqlStr.isEmpty())
        return nullptr;
    qDebug() << sqlStr;
    return exec(sqlStr);
}
