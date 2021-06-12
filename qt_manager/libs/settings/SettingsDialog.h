#pragma once

#include <QDialog>
#include <QHash>
#include "AbstractSettings.h"

namespace mmind {
namespace Ui {
class SettingsDialog;
}

class SETTINGS_EXPORT SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget* parent = nullptr);
    ~SettingsDialog();

    void addPage(AbstractSettings* page);
    QHash<QString, QVariant>& unsavedSettings() { return _unsavedSettings; }
    int exec() override;

private:
    void updatePage();

private slots:
    void accept() override;
    void reject() override;
    void on_listWidget_currentRowChanged(int currentRow);

private:
    Ui::SettingsDialog* const ui;
    QHash<QString, QVariant> _unsavedSettings;
    QVector<AbstractSettings*> _pages;
};
} // namespace mmind
