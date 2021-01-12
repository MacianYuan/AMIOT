#include "httpgetkey.h"

HttpGetKey::HttpGetKey(QObject *parent) : QObject(parent)
{

}

void HttpGetKey::init()
{
    QByteArray getSecretUrl("http://47.92.125.82:8086/mqtt/getDeviceSecret?device_number=");
    //Device_public::device_number = Device_public::device_number.trimmed();
    qDebug()<<"Device_public::device_number"<<Device_public::device_number;
    getSecretUrl.append(Device_public::device_number);
    qDebug()<< "MainWindow::on_get_aliyun_key：" <<getSecretUrl;
    m_manager = new QNetworkAccessManager(this);
    connect(m_manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(slot_replayFinished(QNetworkReply*))); //关联信号和槽
    if(!Device_public::device_number.isEmpty())
        m_manager->get(QNetworkRequest(QUrl(getSecretUrl.data())));//发送请求

}


/******************************************************************************************
* @Function         :slot_replayFinished()
*
* @Purpose          :(2.b)解析秘钥   登录阿里云服务器
* @Description      :
********************* HTTP获取到内容解析出秘钥 和设备key
********************* 出发登录阿里云服务器信号
*********************
* @Grammar&Demand   :
*********************
*********************
* @Param            :
* @Retval           :.00000000
* @Others           :
* @e.g.             :
******************************************************************************************/
void HttpGetKey::slot_replayFinished(QNetworkReply *reply)
{
    QString ReadData = reply->readAll();
    qDebug()<< ReadData;
    //获取不到容错处理
    //HTTP获取，解析字段
    QString devicesecret = "DeviceSecret";
    QString productkey = "ProductKey";
    QString devicename = "DeviceName";

    if(ReadData.startsWith("{")&&ReadData.endsWith("}")){
        int index = ReadData.indexOf(devicesecret)+15;
        int len = ReadData.indexOf(productkey)-index - 3;
        Device_public::DeviceSecret = ReadData.mid(index,len);
        //Device_public::DeviceSecret = "e78e3e4b572972528630f7c16b6e079b";
        qDebug()<<  Device_public::DeviceSecret;
        index = ReadData.indexOf(productkey)+13;
        len = ReadData.indexOf(devicename)-index - 3;
        Device_public::ProductKey = ReadData.mid(index,len);
        //Device_public::ProductKey = "a1LOqD9O9II";
        qDebug()<<  Device_public::ProductKey;
    }
    reply->deleteLater();//最后要释放replay对象
}
