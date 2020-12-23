#ifndef GPSANALYSIS_H
#define GPSANALYSIS_H

#include <QObject>
#include <QStringList>
#include "qextserial/qextserialport.h"
#include "public/device_public.h"
#include <QDebug>
#include <QTimer>

class GpsAnalysis : public QObject
{
    Q_OBJECT
public:
    explicit GpsAnalysis(QObject *parent = 0);
    int WriteCom(QextSerialPort *write_com,QString str);//return write whether successed

    ~GpsAnalysis();
private:
       unsigned  int gps_count1;
       unsigned  int gps_count2;
       unsigned  int gps_count3;
       QTimer *timer;
       QByteArray baRcvData;
       void Gps_set();
signals:
       void tts_message(unsigned int ,QString str);
public slots:
     void ReadCom();
     void on_timeout();
    
};

#endif // GPSANALYSIS_H
