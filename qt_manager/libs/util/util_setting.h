#pragma once
#include <QSettings>

namespace mmind {
QSettings& mmindSettings();

void setAppSetting(const QString& key, const QVariant& value);
} // namespace mmind
