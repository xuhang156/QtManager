#include <QLineSeries>
#include <QChart>
#include <QValueAxis>
#include <QDebug>
#include <QChartView>

#include "ResultsChartWidget.h"
#include "ui_ResultsChartWidget.h"
#include "RrSus018DReportFile.h"

void setLegend(QChart* chart, QString legendName = "");
const QString fontFamily = QStringLiteral("微软雅黑");

ResultsChartWidget::ResultsChartWidget(QWidget* parent)
    : QWidget(parent), ui(new Ui::ResultsChartWidget)
{
    ui->setupUi(this);
}

ResultsChartWidget::~ResultsChartWidget() { delete ui; }

bool ResultsChartWidget::addLineChart(QString title, const QList<QPointF>& points,
                                      const QColor& color, int row, int col)
{
    auto chart = createChart(title);
    addLineseries(chart, points, color);
    addChartToWidget(chart, row, col);
    return true;
}

void setLegend(QChart* chart, QString legendName)
{
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    QFont font = chart->legend()->font();
    font.setFamily(fontFamily);
    font.setItalic(!font.italic());
    chart->legend()->setFont(font);
    font.setPointSizeF(12);

    chart->legend()->setFont(font);
}

void setTile(QChart* chart, QString title)
{
    chart->setTitle(title);
    chart->setTitleBrush(QBrush(QColor(255, 170, 255)));
    chart->setTitleFont(QFont(fontFamily));
}

QChart* ResultsChartWidget::createChart(QString title)
{
    QChart* chart = new QChart();
    if (chart == nullptr)
        return nullptr;

    chart->legend()->setVisible(false);
    chart->setTheme(QChart::ChartThemeBlueCerulean); //设置系统主题
    chart->setAnimationOptions(QChart::NoAnimation); //设置启用或禁用动画
    setTile(chart, title);
    return chart;
}

void ResultsChartWidget::createReportChart(QSet<QString> tasks)
{
    //测试
    const QStringList tableColumnKeys = {
        QStringLiteral("pointIndex"),    QStringLiteral("type"),
        QStringLiteral("longAxis"),      QStringLiteral("shortAxis"),
        QStringLiteral("holePositionL"), QStringLiteral("holePositionw"),
        QStringLiteral("holePositionh"), QStringLiteral("surfacePosition")};
    const QString taskId = QStringLiteral("taskId");

    QHash<QString, QList<QPointF>> pointDatas1;
    int rows = 0;
    qDebug() << "create Chart:" << tasks;
    for (auto iter : tasks) {
        AbstractReportData* datas = new RrSus018DData(QStringLiteral("FrSus_018D"));
        datas->setResultKeys(tableColumnKeys);
        datas->setQueryConditions(taskId, iter);
        datas->readData();
        auto dbData = datas->getAllDatas();
        auto findIter = dbData.begin();
        while (findIter != dbData.end()) {
            QString key = findIter.key();
            auto findHistory = pointDatas1.find(key);
            if (findHistory != pointDatas1.end()) {
                findHistory.value().append(
                    QPointF(findHistory.value().at(findHistory.value().size() - 1).x() + 1,
                            findIter.value()[2].toDouble()));
            } else {
                pointDatas1[key] = {QPointF(0, findIter.value()[2].toDouble())};
            }
            findIter++;
        }
    }
    auto dbIter = pointDatas1.begin();
    int row = 0, col = 0;
    while (dbIter != pointDatas1.end()) {
        if (row == 3) {
            row = 0;
            col++;
        }
        addLineChart(dbIter.key(), dbIter.value(), QColor(255, 2, 0), col, row++);
        dbIter++;
    }
}

bool ResultsChartWidget::addChartToWidget(QChart* chart, int row, int col)
{
    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setFixedSize(QSize(300, 400));
    ui->verticalLayout_3->addWidget(chartView);
    return true;
}

QLineSeries* createLineSeries(const QList<QPointF>& points, const QColor& color)
{
    QLineSeries* lineseries = new QLineSeries;
    lineseries->setColor(color);
    lineseries->setVisible(true);
    lineseries->setPointsVisible(true);
    *lineseries << points;
    return lineseries;
}

bool ResultsChartWidget::addLineseries(QChart* chart, const QList<QPointF>& points,
                                       const QColor& color)
{
    if (chart == nullptr)
        return false;
    chart->addSeries(createLineSeries(points, color));
    chart->addSeries(createLineSeries(points, color));
    chart->createDefaultAxes();
    return true;
}
