#pragma once
#include "AbstractSettings.h"
#include "settings_global.h"
#include "BaseTableValueSettings.h"

namespace mmind {
namespace Ui {
class ReportIndexSettingsPage;
}

class SETTINGS_EXPORT ReportIndexSettingsPage : public AbstractSettings
{
    Q_OBJECT

public:
    explicit ReportIndexSettingsPage(QHash<QString, QVariant>& unsavedSettings,
                                     QWidget* parent = nullptr);
    ~ReportIndexSettingsPage();
    void resetSettings() override;

private:
    Ui::ReportIndexSettingsPage* ui;
    QSharedPointer<BaseTableValueSettins> _tableSettings;
    QHash<QString, QVariant>& _unsavedSettings;
};
} // namespace mmind
