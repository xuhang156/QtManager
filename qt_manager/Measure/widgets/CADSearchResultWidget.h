#pragma once
#include <QSet>
#include <QWidget>

namespace Ui {
class CADSearchResultWidget;
}

struct CADResourceInfo
{
    QString model;
    QString productId;
    QString pdfPath;
    QString excelPath;
    QString updateTime;
    QStringList toStringList() { return QStringList() << updateTime << model << productId; }
};

class CADSearchResultWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CADSearchResultWidget(QWidget* parent = nullptr);
    ~CADSearchResultWidget();

public slots:
    void search(QString models, QString products);
    void handleCheckBoxClicked(int isSelected);
    void on_back_pressed();
    void on_cad_pressed();
    void on_measurement_pressed();

signals:
    void backToPrevious();

private:
    void insertRow();

private:
    Ui::CADSearchResultWidget* ui;
    QSet<QString> _selectedProducts;
    QHash<QString, CADResourceInfo> _resourceInfo;
};
