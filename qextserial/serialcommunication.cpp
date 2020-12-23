/******************************************************************************************
*-----First Created. time: 28-June-2020	3:30  P.m.
* @File name    :Serialcommunication.cpp
* @Platform     :Hi3520D - linux-3.0.8 - Qt4.8.6 - arm-hisiv100nptl-linux-gcc
* @Author       :MacianYuan
* @E-mail       :macian@foxmail.com
* @Project      :农机作业远程监控终端
*******************************************************************************************
* @Description  :
* @Attention    :
* @Log          :
*****************29-June-2020(MacianYuan):
*****************
* @Copyright© @MacianYuan Anhui Xinjie Technology Co.,Ltd.  2010-2020  All rights reserved.
******************************************************************************************/

#include "serialcommunication.h"
#include <stdio.h>

static QByteArray BufferData,PasteData,ReadData,AddData;

SerialCommunication::SerialCommunication(QObject *parent):
    QObject(parent)
{
    //串口初始化
    Device_public::key_port = new QextSerialPort("/dev/ttyAMA2",QextSerialPort::EventDriven);
    //注意：得要先打开串口，然后再设置串口的参数，不然设置无效！！！
    int m_fd = Device_public::key_port->open(QIODevice::ReadWrite);
    //定义串口对象，并传递参数，在构造函数里对其进行初始化
    if(m_fd){
        //Device_public::key_port->setDtr(true);
        //Device_public::key_port->setRts(false);
        //设置波特率
        Device_public::key_port->setBaudRate(BAUD115200);
        //设置数据位
        Device_public::key_port->setDataBits(DATA_8);
        //设置奇偶校验
        Device_public::key_port->setParity(PAR_NONE);
        //设置停止位
        Device_public::key_port->setStopBits(STOP_1);
        //设置数据流控制，我们使用无数据流的默认设置
        Device_public::key_port->setFlowControl(FLOW_OFF);
        //设置延时      --Modify 改小点
        Device_public::key_port->setTimeout(10);
        qDebug() <<"Device_public::myCom_open /dev/ttyAMA2 success!";
    }else{
        qDebug () << tr("open serial failed");
    }

    timer = new QTimer(this);
    timer->setInterval(100);
    //有数据读取到，进入数据分析槽函数，分析出数据
    connect(Device_public::key_port, SIGNAL(readyRead()), this, SLOT(ReadCom()));
    connect(timer, SIGNAL(timeout()), this,SLOT(on_timeout()));
}

void SerialCommunication::run()
{

}

//读数据 对数据进行解析，定时100ms的数据
void SerialCommunication::ReadCom()
{
    //启动定时器，接收100毫秒数据
    timer->start();
    baRcvData.append(Device_public::key_port->readAll());
}

//都数据 对数据进行解析
void SerialCommunication::on_timeout()
{
    qDebug()<<"11111";
    timer->stop();
    qDebug()<<"key_scan"<<baRcvData;

    if(baRcvData.contains("0"))//back
    {
        qDebug()<<"BufferData.contains(0)";

    }
    if(baRcvData.contains("1"))//left
    {
        qDebug()<<"BufferData.contains(1)";
        emit stream_change(true);
    }
    if(baRcvData.contains("2"))//up
    {
        qDebug()<<"BufferData.contains(2)";
        emit stackedshowSelect(true);
    }
    if(baRcvData.contains("5"))//menu
    {
        qDebug()<<"BufferData.contains(5)";
        emit menushowSelect();
    }
    if(baRcvData.contains("6"))//right
    {
        qDebug()<<"BufferData.contains(6)";
        emit stream_change(false);
    }
    if(baRcvData.contains("7"))//down
    {
        qDebug()<<"BufferData.contains(7)";
        emit stackedshowSelect(false);
    }
    if(baRcvData.contains("8"))//ok
    {
        qDebug()<<"BufferData.contains(8)";

    }
    baRcvData.clear();
}

int SerialCommunication::WriteCom(QextSerialPort *write_com,QString str)
{
    if(write_com == 0  || !write_com->isOpen()){
        qDebug() << "SerialCommunication write_com Open Failed!";
        return -1;
    }

    int type_num;
    QByteArray array= str.toUtf8();
    type_num = write_com->write(array);


    if(type_num <= 0){
        //QMessageBox::information(0,tr("错误"),tr("通讯中断"),QMessageBox::Ok);
        write_com->close();
    }

    return 0;
}


SerialCommunication::~SerialCommunication()
{

}
