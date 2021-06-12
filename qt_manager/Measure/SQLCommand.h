#pragma once
#include <QHash>
#include <QVariant>
#include <QSqlQuery>

class SQLCommand
{
public:
    SQLCommand(const QString& table) : _tableName(table){};
    virtual ~SQLCommand(){};

    virtual QSharedPointer<QSqlQuery> select(QString sqlStr);
    virtual QSharedPointer<QSqlQuery> select(QStringList resultKeys,
                                             QHash<QString, QString> queryConditions,
                                             bool selectAll = false);

    //    virtual bool add(QHash<QString, QVariant> addDatas) = 0
    //    virtual bool remove(QHash<QString, QVariant> queryConditions) = 0;
    //    virtual bool update(QHash<QString, QVariant> updateDatas,
    //                        QHash<QString, QVariant> queryConditions) = 0;

private:
    QString _tableName;
};
