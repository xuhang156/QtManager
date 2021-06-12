#include "ReportIndexSettingsPage.h"
#include "ui_ReportIndexSettingsPage.h"

namespace mmind {

ReportIndexSettingsPage::ReportIndexSettingsPage(QHash<QString, QVariant>& unsavedSettings,
                                                 QWidget* parent)
    : AbstractSettings(parent), ui(new Ui::ReportIndexSettingsPage),
      _tableSettings(new BaseTableValueSettins()), _unsavedSettings(unsavedSettings)
{
    ui->setupUi(this);
    resetSettings();
}

ReportIndexSettingsPage::~ReportIndexSettingsPage() { delete ui; }

void ReportIndexSettingsPage::resetSettings()
{
    setWindowTitle("Report Index");
    _tableSettings->initWidget("Report Index Settings", {"head1", "head2"});
    ui->verticalLayout->addWidget(_tableSettings.data());
}

} // namespace mmind
