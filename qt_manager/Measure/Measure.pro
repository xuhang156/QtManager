QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets\
                                        sql\
                                        webengine\
                                        webenginecore\
                                        webenginewidgets\
                                        webchannel

CONFIG += c++11

TARGET = manager

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    AbstractReportFile.cpp \
    ResultsChartWidget.cpp \
    RrSus018DReportFile.cpp \
    SQLCommand.cpp \
    main.cpp \
    MainWindow.cpp \
    widgets/CADSearchResultWidget.cpp \
    widgets/CADWidget.cpp \
    widgets/CalendarWidget.cpp \
    widgets/LoginDialog.cpp \
    widgets/MeasurementResultsSearchWidget.cpp \
    widgets/MeasurementResultsWidget.cpp \
    widgets/ProductBrandStandardWidget.cpp \
    widgets/SettingsWidget.cpp \
    widgets/TabWidget.cpp

HEADERS += \
    AbstractReportFile.h \
    MainWindow.h \
    ResultsChartWidget.h \
    RrSus018DReportFile.h \
    SQLCommand.h \
    widgets/CADSearchResultWidget.h \
    widgets/CADWidget.h \
    widgets/CalendarWidget.h \
    widgets/LoginDialog.h \
    widgets/MeasurementResultsSearchWidget.h \
    widgets/MeasurementResultsWidget.h \
    widgets/ProductBrandStandardWidget.h \
    widgets/SettingsWidget.h \
    widgets/TabWidget.h

FORMS += \
    MainWindow.ui \
    ResultsChartWidget.ui \
    widgets/CADSearchResultWidget.ui \
    widgets/CADWidget.ui \
    widgets/LoginDialog.ui \
    widgets/MeasurementResultsSearchWidget.ui \
    widgets/MeasurementResultsWidget.ui \
    widgets/ProductBrandStandardWidget.ui \
    widgets/SettingsWidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources/language/language.qrc \
    resources/rc/darkstyle/style.qrc

DISTFILES += \
    resources/language/measure_en_US.ts \
    resources/language/measure_ja_JP.ts \
    resources/language/measure_zh_CN.ts

LIBS += -L$$PWD/libs/libxl/lib/ -llibxl
Release:LIBS += -L$$OUT_PWD/../libs/qpdf/release/ -lqpdf
Debug:LIBS += -L$$OUT_PWD/../libs/qpdf/debug/ -lqpdf
Release:LIBS += -L$$OUT_PWD/../libs/settings/release/ -lmmind_settings
Debug:LIBS += -L$$OUT_PWD/../libs/settings/debug/ -lmmind_settings
Release:LIBS += -L$$OUT_PWD/../libs/util/release/ -lmmind_util
Debug:LIBS += -L$$OUT_PWD/../libs/util/debug/ -lmmind_util

INCLUDEPATH += $$PWD/libs/libxl/include \
               $$PWD/../libs/qpdf       \
               $$PWD/../libs/settings   \
               $$PWD/../libs/util       \
               $$PWD/widgets

DEPENDPATH += $$PWD/libs/libxl/include
