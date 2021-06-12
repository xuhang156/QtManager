#pragma once

#include <QDialog>

namespace Ui {
class ProductBrandStandardWidget;
}

class ProductBrandStandardWidget : public QDialog
{
    Q_OBJECT

public:
    explicit ProductBrandStandardWidget(QWidget* parent = nullptr);
    ~ProductBrandStandardWidget();

    void showPDF(const QHash<QString, QString>& pdfPaths);
    void showMeasurementCoordinates(const QStringList& pdfPaths);

private:
    void showPDF(bool isPDF);

private:
    Ui::ProductBrandStandardWidget* ui;
};
