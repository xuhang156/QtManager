#include "MeasurementResultsWidget.h"
#include "ui_MeasurementResultsWidget.h"
#include <SQLCommand.h>
#include "CalendarWidget.h"

std::vector<QDate> getDatesHadMeasured()
{
    std::vector<QDate> dates;
    SQLCommand sql("");
    auto result = sql.select(
        QString("select %1 from %2").arg(QStringLiteral("time"), QStringLiteral("taskStatistics")));
    while (result->next())
        dates.push_back(QDate::fromString(result->value(0).toString(), "yyyy-MM-dd"));
    return dates;
}

MeasurementResultsWidget::MeasurementResultsWidget(QWidget* parent)
    : QWidget(parent), ui(new Ui::MeasurementResultsWidget)
{
    ui->setupUi(this);
    ui->gridLayout->setAlignment(ui->widget, Qt::AlignCenter);

    utilComBoxGroups = mmind::addUtilComboxGroupsItem(ui->formLayout);
    connect(utilComBoxGroups->measurementType, &QComboBox::currentTextChanged, this,
            &MeasurementResultsWidget::on_measurement_type_changed);
    getDatesHadMeasured();
    selectProductMeasure(true);
    auto dates = getDatesHadMeasured();
    initDateEdit(ui->beginDate, dates);
    initDateEdit(ui->endDate, dates);
}

void MeasurementResultsWidget::initDateEdit(QDateEdit* date, const std::vector<QDate> measuredDates)
{
    CalendarWidget* calendarWidget = new CalendarWidget(date, measuredDates);
    date->setDisplayFormat("yyyy-MM-dd");
    date->setCalendarPopup(true);
    date->setDateTime(QDateTime::currentDateTime());
    date->setCalendarWidget(calendarWidget);
    date->setContextMenuPolicy(Qt::NoContextMenu);
    date->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
}

MeasurementResultsWidget::~MeasurementResultsWidget() { delete ui; }

void MeasurementResultsWidget::on_measurement_type_changed(QString text)
{
    mmind::isProductMeasurementType(text) ? selectProductMeasure(true)
                                          : selectProductMeasure(false);
}

void MeasurementResultsWidget::selectProductMeasure(bool isProductMeasure)
{
    setProductInputWidgetsEnabled(isProductMeasure);
    setTrayInputeWidgetsEnabled(!isProductMeasure);
}

void MeasurementResultsWidget::setProductInputWidgetsEnabled(bool enabled)
{
    if (utilComBoxGroups.isNull())
        return;
    utilComBoxGroups->carType->setEnabled(enabled);
    utilComBoxGroups->product->setEnabled(enabled);
}

void MeasurementResultsWidget::setTrayInputeWidgetsEnabled(bool enabled)
{
    ui->trayStart->setEnabled(enabled);
    ui->trayEnd->setEnabled(enabled);
}

void MeasurementResultsWidget::on_search_pressed()
{
    mmind::MeasureType type = mmind::MeasureType::Product;
    if (mmind::TranslatorInstaller::instance().getOriginalStrFromTranslateStr(
            utilComBoxGroups->measurementType->currentText()) == kMeasureMentType[1])
        type = mmind::MeasureType::Tray;

    mmind::SearchInfo info{type, mmind::getSqlStr(utilComBoxGroups->carType->currentText()),
                           mmind::getSqlStr(utilComBoxGroups->product->currentText()),
                           QString("%1 00:00:00").arg(ui->beginDate->text()),
                           QString("%1 23:59:59").arg(ui->endDate->text())};
    emit searchMeasurementResults(info);
}
