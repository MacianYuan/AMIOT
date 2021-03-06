#ifndef MAINSERVICE_H
#define MAINSERVICE_H

#include <QObject>
#include <QTimer>
#include <QApplication>
#include <QTranslator>
#include <QTextCodec>
#include <QThread>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include "hifb.h"
#include "stackedwidget.h"
#include "menuwidget.h"
#include "mainwindow.h"

//#include "rtc/hi_rtc.h"
//#include "rtc/rtc_test.h"
#include "httpgetfile.h"
#include "device_public.h"
#include "httpservice.h"
#include "httpgetkey.h"
#include "irdaanalysis.h"
#include "texttospeech.h"
#include "gpsanalysis.h"
#include "sensoranalysis.h"
#include "serialcommunication.h"
#include "sample_comm.h"

#include "FrameLessWidget/framelesswidget.h"
#include <QtCore/QFileInfoList>

class MainService : public QObject
{
    Q_OBJECT
public:
    explicit MainService(QObject *parent = 0);
    ~MainService();
    void init();
private:
    void tts_message_mode();

signals:
    void service_flow(unsigned int);

    void menushowSelect();
    void stackedshowSelect(bool id);
    void stream_change(bool id);
    void tts_message(unsigned int id ,QString str);
    void label_time_refresh(QString);
    void gps_signal_update();
private slots:
    void on_mainTimeOut();
    void on_service_flow(unsigned int);

    void on_menushowSelect();
    void on_stackedshowSelect(bool id);
    void on_stream_change(bool id);

public slots:
    void on_tts_message(unsigned int id ,QString str);

private:
    QThread * thread_tts;
    TextToSpeech * text_to_speech;

    SensorAnalysis* sensor_analysis;

//    HttpService *httpService;

    QTimer * mainTimer;
    QNetworkAccessManager *m_manager;

//    MqttLogicProcessing *mqttlogic_processing;

};

#endif // MAINSERVICE_H
