#include "ProductInfoSettingsPage.h"
#include "ui_ProductInfoSettingsPage.h"
#include <QDebug>
#include <QFileDialog>

const QString kModel = QStringLiteral("model");
const QString kProduct = QStringLiteral("product");
const QString kPdfPath = QStringLiteral("pdfPath");
const QString kExcelPath = QStringLiteral("excelPath");

namespace mmind {
ProductInfoSettingsPage::ProductInfoSettingsPage(QHash<QString, QVariant>& unsavedSettings,
                                                 QWidget* parent)
    : AbstractSettings(parent), ui(new Ui::ProductInfoSettingsPage),
      _unsavedSettings(unsavedSettings)
{
    ui->setupUi(this);
    resetSettings();
}

ProductInfoSettingsPage::~ProductInfoSettingsPage() { delete ui; }

void ProductInfoSettingsPage::resetSettings()
{
    setWindowTitle("基本信息");
    _unsavedSettings.insert(kProduct, ui->comboBox->currentText());
}

void ProductInfoSettingsPage::on_model_textChanged(const QString& model)
{
    _unsavedSettings.insert(kModel, model);
}

void ProductInfoSettingsPage::on_comboBox_currentTextChanged(const QString& product)
{
    _unsavedSettings.insert(kProduct, product);
}

QString getFileDialogStr(QWidget* parent, QLabel* lineEdit, QString fileType)
{
    QString filePath = QFileDialog::getOpenFileName(parent, QObject::tr("open a file."), "D:/",
                                                    QObject::tr("%1 files(*.%1)").arg(fileType));
    lineEdit->setText(filePath);
    return filePath;
}

void ProductInfoSettingsPage::on_pdfPath_clicked()
{
    QString path = getFileDialogStr(this, ui->pdf, QStringLiteral("pdf"));
    if (!path.isEmpty())
        _unsavedSettings.insert(kPdfPath, path);
}

void ProductInfoSettingsPage::on_excelPath_clicked()
{
    QString path = getFileDialogStr(this, ui->excel, QStringLiteral("xlsx"));
    if (!path.isEmpty())
        _unsavedSettings.insert(kExcelPath, path);
}

} // namespace mmind
