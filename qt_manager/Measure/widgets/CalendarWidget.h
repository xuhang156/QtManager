#pragma once

#include <QCalendarWidget>

class CalendarWidget : public QCalendarWidget
{
public:
    CalendarWidget(QWidget* parent, const std::vector<QDate> specialDates);

protected:
    void paintCell(QPainter* painter, const QRect& rect, const QDate& date) const;

private:
    std::vector<QDate> _curDatas;
};
