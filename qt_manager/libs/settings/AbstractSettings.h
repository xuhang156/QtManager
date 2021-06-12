#pragma once
#include <QWidget>

#include "settings_global.h"

namespace mmind {
    class SETTINGS_EXPORT AbstractSettings : public QWidget {
      public:
        using QWidget::QWidget;
        virtual void resetSettings() = 0;
    };
}  // namespace mmind
