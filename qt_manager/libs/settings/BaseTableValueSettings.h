#pragma once

#include <QWidget>
#include "settings_global.h"

namespace mmind {
namespace Ui {
class BaseTableValueSettins;
}

class SETTINGS_EXPORT BaseTableValueSettins : public QWidget
{
    Q_OBJECT

public:
    explicit BaseTableValueSettins(QWidget* parent = nullptr);
    ~BaseTableValueSettins();
    void initWidget(QString labelName, QStringList tableHeaders);

private slots:
    void on_addItem_pressed();
    void on_removeItem_pressed();

private:
    void insertRow();

private:
    Ui::BaseTableValueSettins* ui;
};
} // namespace mmind
