INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS +=  $$PWD/gpsanalysis.h \
    $$PWD/irdaanalysis.h \
    $$PWD/texttospeech.h \
    $$PWD/sensoranalysis.h \
    $$PWD/serialcommunication.h \
    $$PWD/hiir_codedef.h \
    $$PWD/hiir.h \
    $$PWD/qextserialport.h \
    $$PWD/qextserialport_p.h \
    $$PWD/qextserialport_global.h


SOURCES += $$PWD/gpsanalysis.cpp \
    $$PWD/irdaanalysis.cpp \
    $$PWD/texttospeech.cpp \
    $$PWD/sensoranalysis.cpp \
    $$PWD/serialcommunication.cpp \
    $$PWD/qextserialport.cpp \
    $$PWD/qextserialport_unix.cpp
