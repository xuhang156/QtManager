#pragma once

#include <QSet>
#include "util_gui.h"

namespace Ui {
class MeasurementResultsSearchWidget;
}

class MeasurementResultsSearchWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MeasurementResultsSearchWidget(QWidget* parent = nullptr);
    ~MeasurementResultsSearchWidget();
    void searchDBResult(const mmind::SearchInfo&);

private:
    void insertRow(QString taskId, const QStringList& datas);

signals:
    void backToPrevious();
    void createReportChart(QSet<QString> tasks);

private slots:
    void handleCheckBoxClicked(int isSelected);
    void on_report_pressed();
    void on_back_pressed();

private:
    bool isAccuracyTable;
    bool isAccuracyChart;
    Ui::MeasurementResultsSearchWidget* ui;
    QHash<QString, QStringList> _taskDatas;
    QSet<QString> _selectedTaskIds;
};
