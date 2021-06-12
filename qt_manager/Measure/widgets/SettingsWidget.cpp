#include <QTabBar>
#include <QPushButton>
#include <QPainter>
#include <QStyleOptionTab>
#include <QTextOption>

#include "SettingsWidget.h"
#include "TabWidget.h"
#include "ui_SettingsWidget.h"

SettingsWidget::SettingsWidget(QWidget* parent) : QWidget(parent), ui(new Ui::SettingsWidget)
{
    ui->setupUi(this);

    const QString tabTitle[] = {"tab1sdfsfs", "tab2", "tab3"};
    TabWidget* tabWidget = new TabWidget;
    for (auto title : tabTitle)
        tabWidget->addTab(new QWidget, title);

    ui->verticalLayout->addWidget(tabWidget);
}

SettingsWidget::~SettingsWidget() { delete ui; }
