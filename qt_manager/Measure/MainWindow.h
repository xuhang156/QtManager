#pragma once
#include <QMainWindow>
#include "util_gui.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void openMeasurementResultsSearchWidget(mmind::SearchInfo search);

private:
    QSharedPointer<QWidget> _cadWidget;
    QSharedPointer<QWidget> _cadSearchWidget;
    QSharedPointer<QWidget> _measurementResult;
    QSharedPointer<QWidget> _measurementResultSearch;
    QSharedPointer<QWidget> _resultsChartWidget;
    QSharedPointer<QWidget> _settingsWidget;
    Ui::MainWindow* ui;
};
