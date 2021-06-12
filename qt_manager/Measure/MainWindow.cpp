#include <QTabBar>
#include <QDebug>
#include "MainWindow.h"
#include "CADWidget.h"
#include "CADSearchResultWidget.h"
#include "CalendarWidget.h"
#include "MeasurementResultsWidget.h"
#include "MeasurementResultsSearchWidget.h"
#include "ResultsChartWidget.h"
#include "SettingsWidget.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), _cadWidget(new CADWidget(this)),
      _measurementResult(new MeasurementResultsWidget(this)),
      _measurementResultSearch(new MeasurementResultsSearchWidget(this)),
      _resultsChartWidget(new ResultsChartWidget(this)), _settingsWidget(new SettingsWidget(this)),
      _cadSearchWidget(new CADSearchResultWidget(this))
{
    ui->setupUi(this);

    ui->tabWidget->tabBar()->setObjectName("top_bottom_tab");
    ui->verticalLayout_2->addWidget(_measurementResult.get(), Qt::AlignRight);
    ui->verticalLayout_2->addWidget(_measurementResultSearch.get(), Qt::AlignRight);
    ui->verticalLayout_3->addWidget(_cadWidget.get(), Qt::AlignRight);
    ui->verticalLayout_3->addWidget(_cadSearchWidget.get(), Qt::AlignRight);
    ui->verticalLayout_4->addWidget(_resultsChartWidget.get(), Qt::AlignRight);
    ui->layoutSettings->addWidget(_settingsWidget.get());

    connect(dynamic_cast<MeasurementResultsWidget*>(_measurementResult.get()),
            &MeasurementResultsWidget::searchMeasurementResults, this,
            &MainWindow::openMeasurementResultsSearchWidget);

    connect(dynamic_cast<MeasurementResultsSearchWidget*>(_measurementResultSearch.get()),
            &MeasurementResultsSearchWidget::backToPrevious, this,
            [=]() { _measurementResult->show(); });
    connect(dynamic_cast<MeasurementResultsSearchWidget*>(_measurementResultSearch.get()),
            &MeasurementResultsSearchWidget::createReportChart,
            dynamic_cast<ResultsChartWidget*>(_resultsChartWidget.get()),
            &ResultsChartWidget::createReportChart);

    connect(dynamic_cast<CADWidget*>(_cadWidget.get()), &CADWidget::searchBrandStandard,
            dynamic_cast<CADSearchResultWidget*>(_cadSearchWidget.get()),
            &CADSearchResultWidget::search);

    connect(dynamic_cast<CADSearchResultWidget*>(_cadSearchWidget.get()),
            &CADSearchResultWidget::backToPrevious, this, [=]() { _cadWidget->show(); });
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::openMeasurementResultsSearchWidget(mmind::SearchInfo search)
{
    dynamic_cast<MeasurementResultsSearchWidget*>(_measurementResultSearch.get())
        ->searchDBResult(search);

    _measurementResult->hide();
    _measurementResultSearch->show();
}
