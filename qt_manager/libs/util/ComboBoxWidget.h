#pragma once
#include <QComboBox>
#include <QListView>
#include "util_global.h"

class UTIL_EXPORT ComboBoxWidget : public QComboBox
{
    Q_OBJECT

public:
    explicit ComboBoxWidget(const QStringList& texts, QWidget* parent = nullptr);
    ~ComboBoxWidget() {}
    bool eventFilter(QObject* watched, QEvent* event) override;
    void hidePopup() override;
    void showPopup() override;
    void setDelimiter(QString str) { _delimiter = str; }

private:
    QString _delimiter = ",";

signals:
    void selectedText(QStringList);
};
