#pragma once

#include "AbstractSettings.h"
#include "settings_global.h"

namespace mmind {
namespace Ui {
class ProductInfoSettingsPage;
}

class SETTINGS_EXPORT ProductInfoSettingsPage : public AbstractSettings
{
    Q_OBJECT

public:
    explicit ProductInfoSettingsPage(QHash<QString, QVariant>& unsavedSettings,
                                     QWidget* parent = nullptr);
    ~ProductInfoSettingsPage();
    void resetSettings() override;

private slots:
    void on_pdfPath_clicked();
    void on_excelPath_clicked();
    void on_model_textChanged(const QString& model);
    void on_comboBox_currentTextChanged(const QString&);

private:
    Ui::ProductInfoSettingsPage* ui;
    QHash<QString, QVariant>& _unsavedSettings;
};
} // namespace mmind
