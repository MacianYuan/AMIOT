INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS +=  $$PWD/hisivio.h \
            $$PWD/hisiservice.h

SOURCES += \
    $$PWD/common/loadbmp.c \
    $$PWD/common/sample_comm_sys.c \
    $$PWD/common/sample_comm_vda.c \
    $$PWD/common/sample_comm_vdec.c \
    $$PWD/common/sample_comm_venc.c \
    $$PWD/common/sample_comm_vi.c \
    $$PWD/common/sample_comm_vo.c \
    $$PWD/common/sample_comm_vpss.c \
    $$PWD/hisivio.c \
    $$PWD/hisiservice.cpp

