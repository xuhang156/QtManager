#include "StandardValueSettingsPage.h"
#include "ui_StandardValueSettingsPage.h"

const QStringList tableWidgetHeader = {
    QStringLiteral("Point Name"), QStringLiteral("Measurement item"),
    QStringLiteral("Standard value"), QStringLiteral("Upper limit"), QStringLiteral("Lower limit")};

namespace mmind {
StandardValueSettingsPage::StandardValueSettingsPage(QHash<QString, QVariant>& unsavedSettings,
                                                     QWidget* parent)
    : AbstractSettings(parent), ui(new Ui::StandardValueSettingsPage),
      _tableSettings(new BaseTableValueSettins()), _unsavedSettings(unsavedSettings)
{
    ui->setupUi(this);
    resetSettings();
}

StandardValueSettingsPage::~StandardValueSettingsPage() { delete ui; }

void StandardValueSettingsPage::resetSettings()
{
    setWindowTitle("Standard Value");
    _tableSettings->initWidget("Standard Value Settings", tableWidgetHeader);
    ui->verticalLayout->addWidget(_tableSettings.data());
}

} // namespace mmind
