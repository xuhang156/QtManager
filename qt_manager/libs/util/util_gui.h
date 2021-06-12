#pragma once

#include <QObject>
#include <QCheckBox>
#include <QComboBox>
#include <QTableWidget>
#include <QHeaderView>
#include <QTranslator>
#include <QMetaType>
#include <QSettings>
#include <QFormLayout>

#include "util_global.h"

UTIL_EXPORT extern const QString kMeasureOrganization;
UTIL_EXPORT extern const QString kTranslations;
UTIL_EXPORT extern const QStringList kMeasureMentType;

namespace mmind {

UTIL_EXPORT QSettings& mmindSettings();
UTIL_EXPORT QString languageConfigValue();

enum MeasureType { Product, Tray };

struct UTIL_EXPORT SearchInfo
{
    MeasureType measurementType;
    QString carType;
    QString products;
    QString startTime;
    QString endTime;
};

struct UTIL_EXPORT UtilComboBoxGroups
{
    QComboBox* measurementType;
    QComboBox* carType;
    QComboBox* product;
};

class UTIL_EXPORT TranslatorInstaller
{
public:
    static TranslatorInstaller& instance();
    static bool setTranslateStr(QString translatedStr, QString originalStr);
    static QString getOriginalStrFromTranslateStr(QString translatedStr);
    bool updateLanguage(const QString& language);
    void changeLanguage(QString language);
    void install();

private:
    TranslatorInstaller();

private:
    QScopedPointer<QTranslator> _translator;
    static QHash<QString, QString> _translatedStrToOriginalStr;
};

UTIL_EXPORT QSharedPointer<UtilComboBoxGroups> addUtilComboxGroupsItem(QFormLayout* layout);
UTIL_EXPORT bool setupGui(QApplication*, const QString&);

UTIL_EXPORT const QCheckBox* createCheckBoxToTableWidget(QTableWidget* tableWidget, int row,
                                                         int column, QString objectName);
UTIL_EXPORT QString getSqlStr(QString str);
UTIL_EXPORT QStringList getTranslateStr(QStringList strs);

UTIL_EXPORT bool isProductMeasurementType(QString typeStr);
UTIL_EXPORT void initUtilTableWidgetHeader(QTableWidget* tableWidget, bool needSort = false);

} // namespace mmind
