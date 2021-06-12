#pragma once
#include <QTabBar>
#include <QTabWidget>

class TabBar : public QTabBar
{
public:
    TabBar() { setObjectName("left_right_tab"); }
    QSize tabSizeHint(int index) const;

protected:
    void paintEvent(QPaintEvent* /*event*/);
};

class TabWidget : public QTabWidget
{
public:
    TabWidget(QWidget* parent = 0);
};
