#pragma once

#include <string>
#include <vector>
#include <QVariant>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QRectF>
#include <QByteArray>
#include <QStringList>
#include "util_global.h"

class QObject;
class QMetaProperty;
namespace mmind {
UTIL_EXPORT QJsonDocument loadJson(const QString& jsonPath, bool warnIfNotExists = true,
                                   QString* errorMsg = nullptr);
} // namespace mmind
