#include "gpsanalysis.h"
#include <QElapsedTimer>
GpsAnalysis::GpsAnalysis(QObject *parent) :
    QObject(parent)
{
    //串口初始化
    Device_public::gps_port = new QextSerialPort("/dev/ttyAMA1",QextSerialPort::EventDriven);
    //注意：得要先打开串口，然后再设置串口的参数，不然设置无效！！！
    int m_fd = Device_public::gps_port->open(QIODevice::ReadWrite);
    //定义串口对象，并传递参数，在构造函数里对其进行初始化
    if(m_fd){
        //Device_public::gps_port->setDtr(true);
        //Device_public::gps_port->setRts(false);
        //设置波特率
        Device_public::gps_port->setBaudRate(BAUD9600);
        //设置数据位
        Device_public::gps_port->setDataBits(DATA_8);
        //设置奇偶校验
        Device_public::gps_port->setParity(PAR_NONE);
        //设置停止位
        Device_public::gps_port->setStopBits(STOP_1);
        //设置数据流控制，我们使用无数据流的默认设置
        Device_public::gps_port->setFlowControl(FLOW_OFF);
        //设置延时      --Modify 改小点
        Device_public::gps_port->setTimeout(10);
        qDebug() <<"Device_public::myCom_open /dev/ttyAMA1 success!";
    }else{
        qDebug () << tr("open gps_port serial failed");
    }

    timer = new QTimer(this);
    timer->setInterval(100);

    //GPS 配置
    Gps_set();

    //有数据读取到，进入数据分析槽函数，分析出数据
    connect(Device_public::gps_port, SIGNAL(readyRead()), this,SLOT(ReadCom()));
    connect(timer, SIGNAL(timeout()), this,SLOT(on_timeout()));
}

//读数据 对数据进行解析，定时100ms的数据
void GpsAnalysis::ReadCom()
{
    //启动定时器，接收100毫秒数据
    timer->start();
    baRcvData.append(Device_public::gps_port->readAll());
}


//都数据 对数据进行解析
void GpsAnalysis::on_timeout()
{
    QString RMC_data;
    QString GGA_data;
    QString VTG_data;
    qDebug()<<"GPS data receive"<<baRcvData.length() <<baRcvData;
    timer->stop();
    //长度不为183 即为不完整数据   不进入判断 （修复QStringList index out of range）
    if(baRcvData.length()>=170)
    {
        gps_count3 = 0;
        if(baRcvData.startsWith("$")){
            RMC_data = baRcvData.mid(baRcvData.indexOf("$GNRMC,"),baRcvData.indexOf("$GNGGA,"));
            GGA_data = baRcvData.mid(baRcvData.indexOf("$GNGGA,"),baRcvData.indexOf("$GNVTG,"));
            VTG_data = baRcvData.mid(baRcvData.indexOf("$GNVTG,"),baRcvData.size());
        }
        QStringList RMC_list = RMC_data.split(",");//QString字符串分割函数
        QStringList GGA_list = GGA_data.split(",");//QString字符串分割函数
        QStringList VTG_list = VTG_data.split(",");//QString字符串分割函数

        if((VTG_list.size()>=7) && (GGA_data.size()>=7) && (RMC_data.size()>=6)){

            Device_public::gps_information.GPS_status = RMC_list.at(2);

            qDebug()<<"Device_public::gps_information.GPS_status"<<Device_public::gps_information.GPS_status;

            //有效定位  定位成功
            //$GNRMC,114700.00,A,3150.00888,N,11708.07137,E,1.474,,240820,,,A,V*1F
            //$GNGGA,114700.00,3150.00888,N,11708.07137,E,1,07,1.31,99.0,M,-2.4,M,,*6C
            //$GNVTG,,T,,M,1.474,N,2.731,K,A*3C
            if(Device_public::gps_information.GPS_status.contains("A")){
                Device_public::gps_information.GPS_Latitude = RMC_list.at(3).toDouble()/100;  //y
                Device_public::gps_information.GPS_Latitude_direction = RMC_list.at(4);
                Device_public::gps_information.GPS_Longitude = RMC_list.at(5).toDouble()/100; //x
                Device_public::gps_information.GPS_Longitude_direction = RMC_list.at(6);
                Device_public::gps_information.GPS_num_satellites = GGA_list.at(7).toInt(0,10);
                Device_public::gps_information.GPS_speed = VTG_list.at(7);
                Device_public::state_GPS = true;
                qDebug()<<""<<Device_public::gps_information.GPS_Latitude ;
                qDebug()<<""<<Device_public::gps_information.GPS_Longitude ;
                qDebug()<<""<<Device_public::gps_information.GPS_num_satellites ;
                qDebug()<<""<<Device_public::gps_information.GPS_speed ;
            }
            //无效定位 正在定位
            else if(Device_public::gps_information.GPS_status.contains("V")){
                qDebug()<< Device_public::gps_information.GPS_status;

                if(gps_count1 >=30){
                    //GPS无效定位
                    emit tts_message(1,"65E065485B9A4F4D4E258C284F5C4E1A");
                    Device_public::state_GPS = false;
                    qDebug()<< "GPS To locate failure"<<gps_count1;

                        gps_count1 = 0;
                }

                gps_count1 ++;
            }
        }
        //获取GPS信息失败
        else {
                if(gps_count2 >=30){
                    //GPS定位失败
                    emit tts_message(1,"5B9A4F4D59318D254E258C284F5C4E1A");
                    Device_public::state_GPS = false;
                    qDebug()<< "GPS To locate failure";

                        gps_count2 = 0;
                }
                gps_count2 ++;
        }
    }else{
        if(gps_count3 >=30){
            //GPS定位失败
            emit tts_message(1,"5B9A4F4D59318D254E258C284F5C4E1A");
             Device_public::state_GPS = false;
            qDebug()<< "GPS To locate failure";

                gps_count3 = 0;
        }
        gps_count3 ++;

    }
    baRcvData.clear();
}


void GpsAnalysis::Gps_set()
{
    WriteCom(Device_public::gps_port,"$CCINV,2500,*50\r\n");
    WriteCom(Device_public::gps_port,"$CCMSG,RMC,1,1,*05\r\n");
    WriteCom(Device_public::gps_port,"$CCMSG,VTG,1,1,*1C\r\n");
    WriteCom(Device_public::gps_port,"$CCMSG,GGA,1,1,*19\r\n");

    WriteCom(Device_public::gps_port,"$CCMSG,GSA,1,0,*0D\r\n");
    WriteCom(Device_public::gps_port,"$CCMSG,GSV,1,0,*1A\r\n");
    WriteCom(Device_public::gps_port,"$CCMSG,GLL,1,0,*1F\r\n");
    WriteCom(Device_public::gps_port,"$CCMSG,ZDA,1,0,*07\r\n");
    WriteCom(Device_public::gps_port,"$CCMSG,DTM,1,0,*05\r\n");
    WriteCom(Device_public::gps_port,"$CCMSG,GNS,1,0,*02\r\n");
    WriteCom(Device_public::gps_port,"$CCMSG,GBS,1,0,*0E\r\n");
    WriteCom(Device_public::gps_port,"$CCMSG,GRS,1,0,*1E\r\n");
    WriteCom(Device_public::gps_port,"$CCMSG,GST,1,0,*18\r\n");
    WriteCom(Device_public::gps_port,"$CCMSG,TXT,1,0,*00\r\n");
}

int GpsAnalysis::WriteCom(QextSerialPort *write_com,QString str)
{
    if(write_com == 0  || !write_com->isOpen()){
        qDebug() << "GpsAnalysis write_com Open Failed!";
        return -1;
    }
    int type_num;
    QByteArray array= str.toUtf8();
    type_num = write_com->write(array);

    if(type_num <= 0){
        //QMessageBox::information(0,tr("错误"),tr("通讯中断"),QMessageBox::Ok);
        write_com->close();
        return -1;
    }
    return 0;
}


GpsAnalysis::~GpsAnalysis()
{

}
