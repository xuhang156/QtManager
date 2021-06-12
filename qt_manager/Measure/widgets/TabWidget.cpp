#include <QPainter>
#include <QStyleOptionTab>
#include <QTextOption>
#include <QStyleOptionTab>
#include <QStylePainter>
#include "TabWidget.h"

QSize TabBar::tabSizeHint(int index) const
{
    QSize size = QTabBar::tabSizeHint(index);
    size.transpose();
    return size;
}

void TabBar::paintEvent(QPaintEvent* /*event*/)
{
    QStylePainter painter(this);
    QStyleOptionTab opt;

    for (int i = 0; i < count(); i++) {
        initStyleOption(&opt, i);
        painter.drawControl(QStyle::CE_TabBarTabShape, opt);
        painter.save();

        QSize rectSize = opt.rect.size();
        rectSize.transpose();
        QRect rect(QPoint(), rectSize);
        rect.moveCenter(opt.rect.center());
        opt.rect = rect;

        QPoint center = tabRect(i).center();
        painter.translate(center);
        painter.rotate(90);
        painter.translate(-center);
        painter.drawControl(QStyle::CE_TabBarTabLabel, opt);
        painter.restore();
    }
}

TabWidget::TabWidget(QWidget* parent) : QTabWidget(parent)
{
    setTabBar(new TabBar);
    setTabPosition(QTabWidget::West);
}
