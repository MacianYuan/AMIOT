#-------------------------------------------------
#
# Project created by QtCreator 2013-12-12T10:26:26
#   -std=c++11
#-------------------------------------------------

QT       += core gui webkit sql network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

TARGET = AMIOT
TEMPLATE = app

QMAKE_CXXFLAGS +=  -std=c++0x -O0 -Wno-psabi


SOURCES += main.cpp\
        mainwindow.cpp \
    sample/common/loadbmp.c \
    sample/common/sample_comm_audio.c \
    sample/common/sample_comm_sys.c \
    sample/common/sample_comm_vda.c \
    sample/common/sample_comm_vdec.c \
    sample/common/sample_comm_venc.c \
    sample/common/sample_comm_vi.c \
    sample/common/sample_comm_vo.c \
    sample/common/sample_comm_vpss.c \
    menuwidget.cpp \
    stackedwidget.cpp \
    qextserial/serialcommunication.cpp \
    qextserial/qextserialport.cpp \
    qextserial/qextserialport_unix.cpp \
    LinkSDK/ntp_posix.c \
    LinkSDK/components/ntp/aiot_ntp_api.c \
    LinkSDK/core/sysdep/core_sysdep.c \
    LinkSDK/core/utils/core_string.c \
    LinkSDK/core/utils/core_sha256.c \
    LinkSDK/core/utils/core_log.c \
    LinkSDK/core/utils/core_http.c \
    LinkSDK/core/utils/core_global.c \
    LinkSDK/core/utils/core_diag.c \
    LinkSDK/core/utils/core_auth.c \
    LinkSDK/core/aiot_state_api.c \
    LinkSDK/core/aiot_mqtt_api.c \
    LinkSDK/external/mbedtls/library/x509.c \
    LinkSDK/external/mbedtls/library/x509_crt.c \
    LinkSDK/external/mbedtls/library/timing.c \
    LinkSDK/external/mbedtls/library/ssl_tls.c \
    LinkSDK/external/mbedtls/library/ssl_cookie.c \
    LinkSDK/external/mbedtls/library/ssl_cli.c \
    LinkSDK/external/mbedtls/library/ssl_ciphersuites.c \
    LinkSDK/external/mbedtls/library/sha256.c \
    LinkSDK/external/mbedtls/library/sha1.c \
    LinkSDK/external/mbedtls/library/rsa.c \
    LinkSDK/external/mbedtls/library/platform.c \
    LinkSDK/external/mbedtls/library/pkparse.c \
    LinkSDK/external/mbedtls/library/pk.c \
    LinkSDK/external/mbedtls/library/pk_wrap.c \
    LinkSDK/external/mbedtls/library/pem.c \
    LinkSDK/external/mbedtls/library/oid.c \
    LinkSDK/external/mbedtls/library/net_sockets.c \
    LinkSDK/external/mbedtls/library/md.c \
    LinkSDK/external/mbedtls/library/md_wrap.c \
    LinkSDK/external/mbedtls/library/error.c \
    LinkSDK/external/mbedtls/library/entropy.c \
    LinkSDK/external/mbedtls/library/entropy_poll.c \
    LinkSDK/external/mbedtls/library/debug.c \
    LinkSDK/external/mbedtls/library/ctr_drbg.c \
    LinkSDK/external/mbedtls/library/cipher.c \
    LinkSDK/external/mbedtls/library/cipher_wrap.c \
    LinkSDK/external/mbedtls/library/bignum.c \
    LinkSDK/external/mbedtls/library/base64.c \
    LinkSDK/external/mbedtls/library/asn1parse.c \
    LinkSDK/external/mbedtls/library/aes.c \
    LinkSDK/portfiles/aiot_port/posix_port.c \
    LinkSDK/aliyunclient.cpp \
    LinkSDK/external/ali_ca_cert.c \
    LinkSDK/cJSON.c \
    LogicalProcessing/mqtt_logic_processing.cpp \
    sample/common/sample_vio.c \
    rtc/rtc_test.c \
    LogicalProcessing/mqtt_thread_process.cpp \
    LogicalProcessing/mqtt_thread_recv.cpp \
    public/IniParse.cpp \
    public/common.cpp \
    qextserial/texttospeech.cpp \
    public/device_public.cpp \
    qextserial/gpsanalysis.cpp \
    qextserial/irdaanalysis.cpp \
    qextserial/sensoranalysis.cpp \
    sample/common/mal.c \
    sample/common/gui.c \
    sqlworker/sqlworker.cpp \
    public/httpservice.cpp \
    mainservice.cpp \
    public/httpgetfile.cpp \
    public/httpgetkey.cpp





HEADERS  += mainwindow.h \
    extdrv/nvp1918/nvp1918.h \
    mal/streamblock.h \
    sample/common/loadbmp.h \
    sample/common/sample_comm.h \
    sample/common/sample_vio.h \
    menuwidget.h \
    stackedwidget.h \
    qextserial/serialcommunication.h \
    qextserial/qextserialport.h \
    qextserial/qextserialport_p.h \
    qextserial/qextserialport_global.h \
    LinkSDK/ntp_posix.h \
    LinkSDK/external/mbedtls/include/mbedtls/x509.h \
    LinkSDK/external/mbedtls/include/mbedtls/x509_crt.h \
    LinkSDK/external/mbedtls/include/mbedtls/x509_crl.h \
    LinkSDK/external/mbedtls/include/mbedtls/timing.h \
    LinkSDK/external/mbedtls/include/mbedtls/ssl.h \
    LinkSDK/external/mbedtls/include/mbedtls/ssl_internal.h \
    LinkSDK/external/mbedtls/include/mbedtls/ssl_cookie.h \
    LinkSDK/external/mbedtls/include/mbedtls/ssl_ciphersuites.h \
    LinkSDK/external/mbedtls/include/mbedtls/sha256.h \
    LinkSDK/external/mbedtls/include/mbedtls/sha1.h \
    LinkSDK/external/mbedtls/include/mbedtls/rsa.h \
    LinkSDK/external/mbedtls/include/mbedtls/platform.h \
    LinkSDK/external/mbedtls/include/mbedtls/pk.h \
    LinkSDK/external/mbedtls/include/mbedtls/pk_internal.h \
    LinkSDK/external/mbedtls/include/mbedtls/pem.h \
    LinkSDK/external/mbedtls/include/mbedtls/oid.h \
    LinkSDK/external/mbedtls/include/mbedtls/net.h \
    LinkSDK/external/mbedtls/include/mbedtls/net_sockets.h \
    LinkSDK/external/mbedtls/include/mbedtls/md5.h \
    LinkSDK/external/mbedtls/include/mbedtls/md.h \
    LinkSDK/external/mbedtls/include/mbedtls/md_internal.h \
    LinkSDK/external/mbedtls/include/mbedtls/error.h \
    LinkSDK/external/mbedtls/include/mbedtls/entropy.h \
    LinkSDK/external/mbedtls/include/mbedtls/entropy_poll.h \
    LinkSDK/external/mbedtls/include/mbedtls/ecp.h \
    LinkSDK/external/mbedtls/include/mbedtls/des.h \
    LinkSDK/external/mbedtls/include/mbedtls/debug.h \
    LinkSDK/external/mbedtls/include/mbedtls/ctr_drbg.h \
    LinkSDK/external/mbedtls/include/mbedtls/config.h \
    LinkSDK/external/mbedtls/include/mbedtls/cipher.h \
    LinkSDK/external/mbedtls/include/mbedtls/cipher_internal.h \
    LinkSDK/external/mbedtls/include/mbedtls/check_config.h \
    LinkSDK/external/mbedtls/include/mbedtls/bn_mul.h \
    LinkSDK/external/mbedtls/include/mbedtls/bignum.h \
    LinkSDK/external/mbedtls/include/mbedtls/base64.h \
    LinkSDK/external/mbedtls/include/mbedtls/asn1.h \
    LinkSDK/external/mbedtls/include/mbedtls/aes.h \
    LinkSDK/core/aiot_sysdep_api.h \
    LinkSDK/core/aiot_state_api.h \
    LinkSDK/core/aiot_mqtt_api.h \
    LinkSDK/core/aiot_http_api.h \
    LinkSDK/core/utils/core_mqtt.h \
    LinkSDK/core/utils/core_sha256.h \
    LinkSDK/core/utils/core_string.h \
    LinkSDK/core/utils/core_log.h \
    LinkSDK/core/utils/core_list.h \
    LinkSDK/core/utils/core_http.h \
    LinkSDK/core/utils/core_global.h \
    LinkSDK/core/utils/core_diag.h \
    LinkSDK/core/utils/core_auth.h \
    LinkSDK/core/sysdep/core_sysdep.h \
    LinkSDK/core/sysdep/core_stdinc.h \
    LinkSDK/components/logpost/logpost_private.h \
    LinkSDK/components/logpost/aiot_logpost_api.h \
    LinkSDK/components/ntp/ntp_private.h \
    LinkSDK/components/ntp/aiot_ntp_api.h \
    LinkSDK/components/ota/ota_private.h \
    LinkSDK/components/ota/ota_md5.h \
    LinkSDK/components/ota/aiot_ota_api.h \
    LinkSDK/aliyunclient.h \
    LinkSDK/external/ali_ca_cert.h \
    LinkSDK/portfiles/aiot_port/posix_port.h \
    LinkSDK/cJSON.h \
    LogicalProcessing/mqtt_logic_processing.h \
    rtc/rtc_test.h \
    LogicalProcessing/mqtt_thread_process.h \
    LogicalProcessing/mqtt_thread_recv.h \
    public/IniParse.h \
    public/common.h \
    public/enum.h \
    public/typedef.h \
    public/version.h \
    qextserial/texttospeech.h \
    public/device_public.h \
    qextserial/gpsanalysis.h \
    qextserial/irdaanalysis.h \
    qextserial/hiir_codedef.h \
    qextserial/hiir.h \
    qextserial/sensoranalysis.h \
    sample/common/mal.h \
    sample/common/gui.h \
    sqlworker/sqlworker.h \
    public/httpservice.h \
    mainservice.h \
    public/httpgetfile.h \
    public/httpgetkey.h




FORMS    += mainwindow.ui \
    menuwidget.ui \
    stackedwidget.ui

INCLUDEPATH += LinkSDK/components/logpost LinkSDK/components/ntp LinkSDK/components/ota
INCLUDEPATH += LinkSDK/core  LinkSDK/core/sysdep LinkSDK/core/utils
INCLUDEPATH += LinkSDK/  LinkSDK/external  LinkSDK/external/mbedtls/include/ LinkSDK/external/mbedtls/include/mbedtls

INCLUDEPATH += /opt/Qt4.8.6-hi3520/hinclude
INCLUDEPATH += $$PWD/extdrv/tw2865 $$PWD/extdrv/tw2960 $$PWD/extdrv/tlv320aic31
INCLUDEPATH += $$PWD/extdrv/cx26828 $$PWD/sample/vio $$PWD/extdrv/nvp1918
#INCLUDEPATH += $$PWD/include

LIBS +=  -lQtGui -lQtNetwork -lQtCore -lpthread
LIBS += -L/opt/Qt4.8.6-hi3520/hilib -lmpi -lm  -lmem -lvqev2  -lhdmi -ltde -laec -ljpeg  -lanr -lresampler -lVoiceEngine -lboost_system

RESOURCES += \
    FrameLessWidget/images.qrc

DISTFILES += \
    LinkSDK/portfiles/SensorProtocol \
    LinkSDK/portfiles/HttpProtocol

#$$PWD/sample/common

OTHER_FILES += \
    LinkSDK/portfiles/README.md
