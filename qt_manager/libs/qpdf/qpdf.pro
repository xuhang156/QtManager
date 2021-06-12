QT -= gui
QT += widgets\
      webengine\
      webenginecore\
      webenginewidgets\
      webchannel

TEMPLATE = lib
TARGET = qpdf
DEFINES += QPDFLIB_BUILD

CONFIG += dll
CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
Release:DLLDESTDIR += ../../Measure/release
Debug:DLLDESTDIR += ../../Measure/debug


HEADERS =\
    qpdfwidget.h \
    pdfjsbridge.h

SOURCES =\
    qpdfwidget.cpp \
    pdfjsbridge.cpp

!isEmpty(target.path): INSTALLS += target
