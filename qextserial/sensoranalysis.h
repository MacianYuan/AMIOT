#ifndef SENSORANALYSIS_H
#define SENSORANALYSIS_H

#include <QObject>
#include <QMutex>
#include <QStringList>
#include "qextserial/qextserialport.h"
#include "device_public.h"
#include <QDebug>
#include <QTimer>
#include <QElapsedTimer>
#include "version.h"
#include <QThread>

#define GET_DEVICE_NUMBER_TIMES 3       //获取机具号3次失败，不再获取
#define GET_DATA_MS_TIME        150     //有数据到达，不断读取数据100ms 确认数据完全接收

class SensorAnalysis : public QThread
{
    Q_OBJECT
public:
    SensorAnalysis(QObject *parent = 0);
    int WriteCom(QextSerialPort *write_com,QByteArray str);//return write whether successed
    int get_device_number();
    int get_sensor1_number();
    int get_sensor2_number();
    int get_sensor3_number();
    int get_sensor4_number();
    int get_sensor_oil_wear();
    int set_sensor_oil_wear();
    void init();
    void stopImmediately();
    void run();
    ~SensorAnalysis();

private:
    bool connect_devicenumber();
    bool connect_sensor();
    //互斥锁
    bool m_isCanRun;
    QMutex m_lock;
    //接收数据定时
    QTimer *timer;
    //数据接收原始字节数组
    QByteArray baRcvData;

signals:
     void tts_message(unsigned int num,QString str);
     //获取到机具识别号，获取阿里云秘钥
     void get_aliyun_key();
     void sensor_capturedata(QString str);
     void sensor_data_analysis(QByteArray str);

private slots:
     void ReadCom();
     void on_timeout();
     void on_sensor_data_analysis(QByteArray str);
};

#endif // SENSORANALYSIS_H
