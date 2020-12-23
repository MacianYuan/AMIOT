#ifndef SERIALCOMMUNICATION_H
#define SERIALCOMMUNICATION_H
#include <QThread>
#include <QObject>
#include <QDebug>
#include <QTimer>
#include <QDataStream>
#include <QMessageBox>
#include "public/device_public.h"
#include "qextserial/qextserialport.h"
#include <qsocketnotifier.h>
#include "cJSON.h"

extern "C"
{

}

class SerialCommunication : public QObject
{
    Q_OBJECT
public:
    explicit SerialCommunication(QObject *parent = 0);

    int WriteCom(QextSerialPort *write_com,QString str);//return write whether successed
    //int InitCom(QextSerialPort *init_com);

    //QTimer* serialreadData;
    ~SerialCommunication();

protected:
    void run();

signals:
    //菜单栏显示 隐藏信号  通过mainwindow 转到menuwidget 槽函数
    void menushowSelect();
    //切换 stackedwidget 页面显示 通过mainwindow 转到mainwindow 槽函数
    void stackedshowSelect(bool);

    void stream_change(bool);
public slots:
    void ReadCom();
    void on_timeout();
private:
    QTimer *timer;
     QByteArray baRcvData;
    //QSocketNotifier *m_notifier;
};

#endif // SERIALCOMMUNICATION_H
