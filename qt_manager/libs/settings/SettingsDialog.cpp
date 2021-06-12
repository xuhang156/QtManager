#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"
//#include "ProductInfoSettingsPage.h"
//#include "StandardValueSettingsPage.h"
//#include "ReportIndexSettingsPage.h"

namespace mmind {

SettingsDialog::SettingsDialog(QWidget* parent) : QDialog(parent), ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    ui->listWidget->setSpacing(4);
    ui->listWidget->setMaximumWidth(140);
    //    addPage(new ProductInfoSettingsPage(unsavedSettings(), this));
    //    addPage(new StandardValueSettingsPage(unsavedSettings(), this));
    //    addPage(new ReportIndexSettingsPage(unsavedSettings(), this));
}

SettingsDialog::~SettingsDialog() { delete ui; }

void SettingsDialog::addPage(AbstractSettings* page)
{
    if (!page)
        return;
    _pages.push_back(page);
    auto* const pageName = new QListWidgetItem(ui->listWidget);
    pageName->setText(page->windowTitle());
    pageName->setTextAlignment(Qt::AlignHCenter);
    pageName->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    ui->stackedWidget->addWidget(page);
}

void SettingsDialog::updatePage()
{
    for (AbstractSettings* page : qAsConst(_pages)) {
        page->resetSettings();
    }
}

void SettingsDialog::accept()
{
    //    QSettings& settings = mmindSettings();
    //    for (auto it = _unsavedSettings.cbegin(), itEnd = _unsavedSettings.cend(); it != itEnd;
    //    ++it) {
    //        settings.setValue(it.key(), it.value());
    //    }
    //    _unsavedSettings.clear();
    QDialog::accept();
}

void SettingsDialog::reject()
{
    updatePage();
    QDialog::reject();
}

int SettingsDialog::exec()
{
    updatePage();
    return QDialog::exec();
}
void SettingsDialog::on_listWidget_currentRowChanged(int currentRow)
{
    ui->stackedWidget->setCurrentIndex(currentRow);
}
} // namespace mmind
#include "moc_SettingsDialog.cpp"
