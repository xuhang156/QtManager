#pragma once

#include "AbstractSettings.h"
#include "settings_global.h"
#include "BaseTableValueSettings.h"

namespace mmind {
namespace Ui {
class StandardValueSettingsPage;
}

class SETTINGS_EXPORT StandardValueSettingsPage : public AbstractSettings
{
    Q_OBJECT

public:
    explicit StandardValueSettingsPage(QHash<QString, QVariant>& unsavedSettings,
                                       QWidget* parent = nullptr);
    ~StandardValueSettingsPage();
    void resetSettings() override;

private:
    Ui::StandardValueSettingsPage* ui;
    QSharedPointer<BaseTableValueSettins> _tableSettings;
    QHash<QString, QVariant>& _unsavedSettings;
};

} // namespace mmind
