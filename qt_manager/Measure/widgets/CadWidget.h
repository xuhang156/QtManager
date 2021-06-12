#pragma once

#include <QWidget>
#include "util_gui.h"

namespace Ui {
class CADWidget;
}

class CADWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CADWidget(QWidget* parent = nullptr);
    ~CADWidget();

signals:
    void searchBrandStandard(QString models, QString products);

private slots:
    void on_search_pressed();
    void on_regist_pressed();

private:
    Ui::CADWidget* ui;
    QSharedPointer<mmind::UtilComboBoxGroups> utilComBoxGroups;
};
