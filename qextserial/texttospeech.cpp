#include "texttospeech.h"
#include <QDebug>
TextToSpeech::TextToSpeech(QObject *parent):
    QObject(parent)
{
    //串口初始化
    Device_public::tts_port = new QextSerialPort("/dev/ttyUSB2",QextSerialPort::EventDriven);
    //注意：得要先打开串口，然后再设置串口的参数，不然设置无效！！！
    int m_fd = Device_public::tts_port->open(QIODevice::ReadWrite);
    //定义串口对象，并传递参数，在构造函数里对其进行初始化
    if(m_fd){
        //Device_public::tts_port->setDtr(true);
        //Device_public::tts_port->setRts(false);
        //设置波特率
        Device_public::tts_port->setBaudRate(BAUD115200);
        //设置数据位
        Device_public::tts_port->setDataBits(DATA_8);
        //设置奇偶校验
        Device_public::tts_port->setParity(PAR_NONE);
        //设置停止位
        Device_public::tts_port->setStopBits(STOP_1);
        //设置数据流控制，我们使用无数据流的默认设置
        Device_public::tts_port->setFlowControl(FLOW_OFF);
        //设置延时      --Modify 改小点
        Device_public::tts_port->setTimeout(10);
        qDebug() <<"Device_public::tts_port /dev/ttyUSB2 success!";
    }else{
        qDebug () << tr("open Device_public::tts_port /dev/ttyUSB2 failed");
    }
    //欢迎使用智纳农机智能检测系统
    WriteCom("AT+QTTS=1,\"6B228FCE4F7F7528667A7EB3519C673A667A80FD68C06D4B7CFB7EDF\"\r\n");

    //有数据读取到，进入数据分析槽函数，分析出数据
    //connect(Device_public::tts_port, SIGNAL(readyRead()), this, SLOT(ReadCom()));
}



void TextToSpeech::on_tts_message(unsigned int num,QString str)
{
    QString str1;
    str1 = QString("AT+QTTS=%1,\"%2\"\r\n").arg(num).arg(str);
    qDebug() << str1;
    WriteCom(str1);
}

void TextToSpeech::on_set_volnm(unsigned int Accuracy_Volnm)
{
    QString str1;
    if(Accuracy_Volnm==0)       str1 = "AT+QTTSETUP=1,2,-29000\r\n";
    else if(Accuracy_Volnm==1)  str1 = "AT+QTTSETUP=1,2,-22576\r\n";
    else if(Accuracy_Volnm==2)  str1 = "AT+QTTSETUP=1,2,-15000\r\n";
    else if(Accuracy_Volnm==3)  str1 = "AT+QTTSETUP=1,2,-9000\r\n";
    else if(Accuracy_Volnm==4)  str1 = "AT+QTTSETUP=1,2,0\r\n";
    else if(Accuracy_Volnm==5)  str1 = "AT+QTTSETUP=1,2,6553\r\n";
    else if(Accuracy_Volnm==6)  str1 = "AT+QTTSETUP=1,2,13106\r\n";
    else if(Accuracy_Volnm==7)  str1 = "AT+QTTSETUP=1,2,19659\r\n";
    else if(Accuracy_Volnm==8)  str1 = "AT+QTTSETUP=1,2,26212\r\n";
    else if(Accuracy_Volnm==9)  str1 = "AT+QTTSETUP=1,2,32767\r\n";
    else if(Accuracy_Volnm==10) str1 = "AT+QTTSETUP=1,2,32767\r\n";

    qDebug() << str1;
    WriteCom(str1);
}

//都数据 对数据进行解析
void TextToSpeech::ReadCom()
{
    QByteArray BufferData;
    if (Device_public::tts_port->bytesAvailable() <= 0){
        qDebug() << "read com = 0";
        return ;
    }

    BufferData = Device_public::tts_port->readAll();
    qDebug()<<BufferData;

}
int TextToSpeech::WriteCom(QString str)
{
    if(Device_public::tts_port == 0  || !Device_public::tts_port->isOpen()){
        qDebug() << "TextToSpeech write_com Open Failed!";
        return -1;
    }

    int type_num;
    QByteArray array= str.toUtf8();
    //qDebug() << array;
    type_num = Device_public::tts_port->write(array);
    qDebug() << type_num<<"The write_com Open success!";

    if(type_num <= 0){
        //QMessageBox::information(0,tr("错误"),tr("通讯中断"),QMessageBox::Ok);
        Device_public::tts_port->close();
    }
    sleep(3);
    return 0;
}


TextToSpeech::~TextToSpeech()
{

}
