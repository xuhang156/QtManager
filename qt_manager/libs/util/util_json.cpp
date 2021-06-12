#include <Windows.h> // For ReplaceFileW
#include <QFile>
#include <QMetaProperty>
#include <QDebug>
#include <QCoreApplication>

#include "util_json.h"
#include "json_keys.h"

namespace mmind {
QHash<QString, QString> jsonToStringHash(const QJsonObject& jo)
{
    QHash<QString, QString> map;
    map.reserve(jo.size());
    for (auto it = jo.constBegin(), itEnd = jo.constEnd(); it != itEnd; ++it) {
        map.insert(it.key(), it.value().toString());
    }
    return map;
}

QJsonDocument loadJson(const QString& jsonPath, bool warnIfNotExists, QString* errorMsg)
{
    QFile file(jsonPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        const auto e = QCoreApplication::translate("mmind_file", "Failed to open %1: %2")
                           .arg(jsonPath, file.errorString());
        if (errorMsg) {
            *errorMsg = e;
        }
        if (warnIfNotExists) {
            qWarning().noquote() << e;
        }
        return {};
    }
    QJsonParseError jsonError{};
    const QJsonDocument jd = QJsonDocument::fromJson(file.readAll(), &jsonError);
    if (jsonError.error != QJsonParseError::NoError) {
        const auto e =
            QCoreApplication::translate("mmind_file", "Failed to parse JSON file %1: %2.")
                .arg(jsonPath, jsonError.errorString());
        if (errorMsg)
            *errorMsg = e;
        qWarning().noquote() << e;
    }
    return jd;
}

} // namespace mmind
