#-------------------------------------------------
#
# Project created by QtCreator 2013-12-12T10:26:26
#   -std=c++11
#-------------------------------------------------

QT       += core gui network
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

LIBS += -ldnvqe -live -lmd  -lmpi -lupvqe -lm -lhdmi -ltde -ljpeg -lVoiceEngine -lQt5Mqtt

//LIBS +=  -lQtGui -lQtNetwork -lQtCore -lpthread
//LIBS += -L/opt/Qt4.8.6-hi3520/hilib -lmpi -lm  -lmem -lvqev2  -lhdmi -ltde -laec -ljpeg  -lanr -lresampler -lVoiceEngine -lboost_system

RESOURCES += \
    FrameLessWidget/images.qrc

DISTFILES += \
    LinkSDK/portfiles/SensorProtocol \
    LinkSDK/portfiles/HttpProtocol

OTHER_FILES += \
    LinkSDK/portfiles/README.md
