#pragma once
#include <QDateEdit>
#include "util_gui.h"

namespace Ui {
class MeasurementResultsWidget;
}

class MeasurementResultsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MeasurementResultsWidget(QWidget* parent = nullptr);
    ~MeasurementResultsWidget();
signals:
    void searchMeasurementResults(mmind::SearchInfo);

private:
    void initDateEdit(QDateEdit* date, const std::vector<QDate> measuredDates);
    void selectProductMeasure(bool isProductMeasure);
    void setProductInputWidgetsEnabled(bool);
    void setTrayInputeWidgetsEnabled(bool);

private slots:
    void on_search_pressed();
    void on_measurement_type_changed(QString text);

private:
    Ui::MeasurementResultsWidget* ui;
    QSharedPointer<mmind::UtilComboBoxGroups> utilComBoxGroups;
};
