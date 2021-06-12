#include <QPainter>
#include "CalendarWidget.h"

void drawBackgroundColor(QPainter* painter, const QRect& rect, const QColor& color,
                         const QDate& date)
{
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(Qt::NoPen);
    painter->setBrush(color);
    painter->drawRect(rect.x(), rect.y(), rect.width(), rect.height());
}

void drawTextColor(QPainter* painter, const QRect& rect, const QColor& color, const QDate& date)
{
    painter->setPen(color);
    painter->drawText(rect, Qt::AlignCenter, QString::number(date.day()));
}

CalendarWidget::CalendarWidget(QWidget* parent, const std::vector<QDate> specialDates)
{
    _curDatas = specialDates;
    setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
}

void CalendarWidget::paintCell(QPainter* painter, const QRect& rect, const QDate& date) const
{
    for (const auto& iter : _curDatas) {
        if (iter == date) {
            drawBackgroundColor(painter, rect, QColor(0, 123, 32, 135), date);
        }
    }
    if (date == selectedDate())
        drawBackgroundColor(painter, rect, QColor(0, 145, 255), date);
    drawTextColor(painter, rect, QColor(255, 255, 255, 255), date);
}
