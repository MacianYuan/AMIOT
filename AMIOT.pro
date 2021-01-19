#-------------------------------------------------
#
# Project created by QtCreator 2013-12-12T10:26:26
#   -std=c++11
#-------------------------------------------------
#webkit webkitwidgets
QT       += core  gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

TARGET = AMIOT
TEMPLATE = app

QMAKE_CXXFLAGS +=  -std=c++0x -O0 -Wno-psabi

SOURCES += main.cpp\
    mainwindow.cpp \
    menuwidget.cpp \
    stackedwidget.cpp \
    mainservice.cpp \

HEADERS  += mainwindow.h \
    menuwidget.h \
    stackedwidget.h \
    mainservice.h \

include(./sample/hisivideo.pri)
include(./tools/tools.pri)
include(./deviceinfo/deviceinfo.pri)
include(./qextserial/qextserial.pri)
include(./http/http.pri)

FORMS    += mainwindow.ui \
    menuwidget.ui \
    stackedwidget.ui

INCLUDEPATH += $$PWD/sample/common
INCLUDEPATH += $$PWD/extdrv/nvp6124
INCLUDEPATH += $$PWD/include

LIBS += -L$$PWD/lib
LIBS += -ldnvqe -live -lmd  -lmpi -lupvqe -lm -lhdmi -ltde -ljpeg -lVoiceEngine

RESOURCES += \
    images/images.qrc

OTHER_FILES += \
    LinkSDK/portfiles/README.md
