#include <QDebug>
#include "util_setting.h"
#include "LoginDialog.h"

namespace mmind {
QSettings& mmindSettings()
{
    static QSettings settings(QSettings::IniFormat, QSettings::UserScope, kOrganization);
    return settings;
}

void setAppSetting(const QString& key, const QVariant& value)
{
    QSettings& settings = mmindSettings();
    settings.setValue(key, value);
}
} // namespace mmind
