#include "ComboBoxWidget.h"
#include <QMouseEvent>
#include <QLineEdit>

ComboBoxWidget::ComboBoxWidget(const QStringList& texts, QWidget* parent) : QComboBox(parent)
{
    setEditable(true);
    lineEdit()->setReadOnly(true);
    view()->viewport()->installEventFilter(this);
    addItems(texts);
    view()->model()->setData(view()->model()->index(0, 0), true, Qt::CheckStateRole);
}

bool ComboBoxWidget::eventFilter(QObject* watched, QEvent* event)
{
    if (event->type() == QEvent::MouseButtonRelease) {
        QModelIndex ind = view()->indexAt(((QMouseEvent*)event)->pos());
        bool checked = view()->model()->data(ind, Qt::CheckStateRole).toBool();
        view()->model()->setData(ind, !checked, Qt::CheckStateRole);
        return true;
    }
    return QObject::eventFilter(watched, event);
}

void ComboBoxWidget::hidePopup()
{
    QStringList values;
    for (int i = 0; i < count(); i++) {
        if (itemData(i, Qt::CheckStateRole).toBool()) {
            values << itemText(i);
            setCurrentIndex(i);
        }
    }
    setCurrentText(values.join(_delimiter));
    emit selectedText(values);
    QComboBox::hidePopup();
}

void ComboBoxWidget::showPopup()
{
    QStringList values = currentText().split(_delimiter);
    for (int i = 0; i < count(); i++) {
        setItemData(i, values.contains(itemText(i)), Qt::CheckStateRole);
    }
    QComboBox::showPopup();
}
