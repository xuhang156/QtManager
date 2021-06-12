#pragma once
#include <QChart>
#include <QLineSeries>
#include <QWidget>
QT_CHARTS_USE_NAMESPACE

namespace Ui {
class ResultsChartWidget;
}

class ResultsChartWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ResultsChartWidget(QWidget* parent = nullptr);
    ~ResultsChartWidget();
    bool addLineChart(QString title, const QList<QPointF>& points, const QColor& color, int row,
                      int col);
public slots:
    void createReportChart(QSet<QString> tasks);

protected:
    QChart* createChart(QString title);
    bool addChartToWidget(QChart* chart, int row, int col);
    bool addLineseries(QChart* chart, const QList<QPointF>& points, const QColor& color);

private:
    Ui::ResultsChartWidget* ui;
};
