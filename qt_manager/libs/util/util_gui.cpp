#include <QDebug>
#include <QDir>
#include <QLabel>
#include <QTranslator>
#include <QApplication>
#include "ComboBoxWidget.h"
#include "util_gui.h"

const QString kMeasureOrganization = QStringLiteral("Mmind/Measure");
const QString kTranslations = QStringLiteral("Translations");
const QStringList kProductType = {QObject::tr("FrSus"), QObject::tr("RrSus")};
const QStringList kMeasureMentType = {QObject::tr("Product measurement result"),
                                      QObject::tr("Tray measurement result")};
const QStringList kCarTypes = {"031D", "018D", "290D"};

QHash<QString, QString> mmind::TranslatorInstaller::_translatedStrToOriginalStr;

namespace mmind {
QSettings& mmindSettings()
{
    static QSettings settings(QSettings::IniFormat, QSettings::UserScope, kMeasureOrganization);
    return settings;
}

QString languageConfigValue() { return mmindSettings().value(kTranslations).toString(); }

TranslatorInstaller& TranslatorInstaller::instance()
{
    static TranslatorInstaller instance;
    return instance;
}

TranslatorInstaller::TranslatorInstaller() {}

bool TranslatorInstaller::setTranslateStr(QString translatedStr, QString originalStr)
{
    _translatedStrToOriginalStr[translatedStr] = originalStr;
    return true;
}

QString TranslatorInstaller::getOriginalStrFromTranslateStr(QString translatedStr)
{
    const auto& iter = _translatedStrToOriginalStr.find(translatedStr);
    if (iter != _translatedStrToOriginalStr.end())
        return iter.value();
    return "";
}

bool TranslatorInstaller::updateLanguage(const QString& language)
{
    if (!language.isEmpty()) {
        mmindSettings().setValue(kTranslations, language);
        return true;
    }
    return false;
}

void TranslatorInstaller::install()
{
    if (!_translator.isNull())
        qApp->removeTranslator(_translator.get());
    QString languageFile = QString("measure_%1.qm").arg(languageConfigValue());
    _translator.reset(new QTranslator(qApp));
    if (_translator->load(languageFile, ":/")) {
        QCoreApplication::installTranslator(_translator.get());
    }
}

void TranslatorInstaller::changeLanguage(QString language)
{
    updateLanguage(language);
    install();
}

QSharedPointer<UtilComboBoxGroups> addUtilComboxGroupsItem(QFormLayout* layout)
{
    QSharedPointer<UtilComboBoxGroups> comboBoxGroups(new UtilComboBoxGroups);

    comboBoxGroups->measurementType = new QComboBox;
    comboBoxGroups->measurementType->addItems(
        {QObject::tr(kMeasureMentType[0].toStdString().c_str()),
         QObject::tr(kMeasureMentType[1].toStdString().c_str())});

    comboBoxGroups->carType = new ComboBoxWidget(kCarTypes);
    comboBoxGroups->product =
        new ComboBoxWidget({QObject::tr(kProductType[0].toStdString().c_str()),
                            QObject::tr(kProductType[1].toStdString().c_str())});

    QStringList translatorStrs;
    translatorStrs << kProductType << kMeasureMentType << kCarTypes;
    for (const auto& iter : translatorStrs)
        TranslatorInstaller::instance().setTranslateStr(QObject::tr(iter.toStdString().c_str()),
                                                        iter);

    layout->insertRow(0, new QLabel(QObject::tr("Result type")), comboBoxGroups->measurementType);
    layout->insertRow(1, new QLabel(QObject::tr("Car type")), comboBoxGroups->carType);
    layout->insertRow(2, new QLabel(QObject::tr("Products")), comboBoxGroups->product);
    return comboBoxGroups;
}

bool setupGui(QApplication* app, const QString& language)
{
    QApplication::setStyle("Fusion");
    {
        QFile f(":/darkstyle.qss");
        if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
            app->setStyleSheet(QString::fromLatin1(f.readAll()));
        }
    }
    TranslatorInstaller::instance().install();
    return true;
}

const QCheckBox* createCheckBoxToTableWidget(QTableWidget* tableWidget, int row, int column,
                                             QString objectName)
{
    auto* const checkBox = new QCheckBox(tableWidget);
    checkBox->setObjectName(objectName);

    QWidget* widget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(checkBox);
    layout->setMargin(0);
    layout->setAlignment(checkBox, Qt::AlignCenter);
    widget->setLayout(layout);
    tableWidget->setCellWidget(row, column, widget);
    return checkBox;
}

QString getSqlStr(QString str)
{
    QString sqlStr;
    for (const auto& iter : str.split(","))
        sqlStr = QString("'%1',%2").arg(
            mmind::TranslatorInstaller::instance().getOriginalStrFromTranslateStr(iter), sqlStr);
    if (sqlStr.isEmpty())
        return "";
    return sqlStr.remove(sqlStr.size() - 1, 1);
}

QStringList getTranslateStr(QStringList strs)
{
    QStringList translateStr;
    for (const auto& iter : strs)
        translateStr.append(
            mmind::TranslatorInstaller::instance().getOriginalStrFromTranslateStr(iter));
    return translateStr;
}

bool isProductMeasurementType(QString typeStr)
{
    return TranslatorInstaller::instance().getOriginalStrFromTranslateStr(typeStr) ==
           kMeasureMentType[0];
}

void initUtilTableWidgetHeader(QTableWidget* tableWidget, bool needSort)
{
    tableWidget->verticalHeader()->hide();
    tableWidget->horizontalHeader()->setStretchLastSection(true);
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    QHeaderView* header = tableWidget->horizontalHeader();
    if (needSort) {
        header->setSortIndicator(2, Qt::AscendingOrder);
        header->setSortIndicatorShown(true);
    }
}

} // namespace mmind
