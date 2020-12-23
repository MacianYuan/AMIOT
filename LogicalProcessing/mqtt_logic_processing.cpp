#include "mqtt_logic_processing.h"
#include "cJSON.h"
#include "public/device_public.h"
#include "external/ali_ca_cert.h"
#include "portfiles/aiot_port/posix_port.h"
#include "LinkSDK/core/aiot_mqtt_api.h"
#include <stdio.h>

MqttLogicProcessing::MqttLogicProcessing(QObject *parent) :
    QObject(parent)
{


}


void MqttLogicProcessing::mqtt_logic_get_mode()
{
    int32_t  res = STATE_SUCCESS;

    /*   TOPIC:
     *   /a1LOqD9O9II/20200701001/user/get_mode
     */

    QByteArray pub_topic_char("/");
    pub_topic_char.append(Device_public::ProductKey);
    pub_topic_char.append("/");
    pub_topic_char.append(Device_public::device_number);
    pub_topic_char.append("/user/get_mode");

    /*  JSON 包:
     *  {
     *      "deviceNumber": "20200701001",
     *      "eventDate":    "2019-03-20 18:13:33"
     *  }
     *
     */

    //  创建JSON 包
    cJSON *event_data = cJSON_CreateObject();

    //  获取当前设备机具号
    QByteArray device_name = Device_public::device_number.toLatin1();

    //  获取当前设备系统时间
    QByteArray time_bytearray = Device_public::timeString.toString("yyyy-MM-dd hh:mm:ss").toLatin1();
    qDebug()<<"time_bytearray: "<<time_bytearray.data();

    cJSON_AddStringToObject(event_data, "deviceNumber",device_name.data());
    cJSON_AddStringToObject(event_data, "eventDate", time_bytearray.data());
    //  字节数组转化
    QByteArray pub_payload_string(cJSON_Print(event_data));

    //qDebug()<< pub_payload_string.data();
    //qDebug()<< pub_payload_string.toBase64().data();

    cJSON_Delete(event_data);
    res =   aiot_mqtt_pub(Device_public::mqtt_handle, pub_topic_char.data(), (uint8_t *)pub_payload_string.toBase64().data(),pub_payload_string.toBase64().size(), 0);
    if (res < 0) {
         printf("aiot_mqtt_sub failed, res: -0x%04X\n", -res);
    }
}


void MqttLogicProcessing::mqtt_logic_post_version()
{
    int32_t  res = STATE_SUCCESS;

    /*   TOPIC:
     *   /a1LOqD9O9II/20200701001/user/post_version
     */

    QByteArray pub_topic_char("/");
    pub_topic_char.append(Device_public::ProductKey);
    pub_topic_char.append("/");
    pub_topic_char.append(Device_public::device_number);
    pub_topic_char.append("/user/post_version");

    /*  JSON 包:
     *  {
     *      "deviceNumber": "20200701001",
     *      "eventDate":    "2019-03-20 18:13:33"
     *      "version":
     *  }
     *
     */

    //  创建JSON 包
    cJSON *event_data = cJSON_CreateObject();

    //  获取当前设备机具号
    QByteArray device_name = Device_public::device_number.toLatin1();
    QByteArray current_Version = Device_public::current_Version.toLatin1();
    //  获取当前设备系统时间
    QByteArray time_bytearray =  Device_public::timeString.toString("yyyy-MM-dd hh:mm:ss").toLatin1();
    //qDebug()<< time_bytearray.data();

    cJSON_AddStringToObject(event_data, "deviceNumber",device_name.data());
    cJSON_AddStringToObject(event_data, "eventDate", time_bytearray.data());
    cJSON_AddStringToObject(event_data, "version", current_Version.data());//设备版本号"01"
    //  字节数组转化
    QByteArray pub_payload_string(cJSON_Print(event_data));

    //qDebug()<< pub_payload_string.data();
    //qDebug()<< pub_payload_string.toBase64().data();

    cJSON_Delete(event_data);
    res =   aiot_mqtt_pub(Device_public::mqtt_handle, pub_topic_char.data(), (uint8_t *)pub_payload_string.toBase64().data(),pub_payload_string.toBase64().size(), 0);
    if (res < 0) {
         printf("aiot_mqtt_sub failed, res: -0x%04X\n", -res);
    }
}



void MqttLogicProcessing::mqtt_logic_work_data()
{
    int32_t  res = STATE_SUCCESS;

    /*   TOPIC:
     *   /a1LOqD9O9II/20200701001/user/work_data
     */

    QByteArray pub_topic_char("/");
    pub_topic_char.append(Device_public::ProductKey);
    pub_topic_char.append("/");
    pub_topic_char.append(Device_public::device_number);
    pub_topic_char.append("/user/work_data");

    /*  JSON 包:
     *  {
     *      "deviceNumber": "20200701001",
     *      "eventDate":    "2019-03-20 18:13:33"
     *  }
     *
     */

    //  创建JSON 包
    cJSON *event_data = cJSON_CreateObject();

    //  获取当前设备机具号
    QByteArray device_name = Device_public::device_number.toLatin1();

    //  获取当前设备系统时间
    QByteArray time_bytearray =  Device_public::timeString.toString("yyyy-MM-dd hh:mm:ss").toLatin1();
    //qDebug()<< time_bytearray.data();

    cJSON_AddStringToObject(event_data, "eventDate", time_bytearray.data());
    cJSON_AddNumberToObject(event_data, "cmd",1);//1是on 0是off “on”开启，“off”关闭  mqtt数据传输开关
    cJSON_AddNumberToObject(event_data, "type", 1);//1有效 2无效 该条上报数据类型
    cJSON_AddStringToObject(event_data, "deviceNumber",device_name.data());
    cJSON_AddNumberToObject(event_data,"x",Device_public::gps_information.GPS_Longitude);
    cJSON_AddNumberToObject(event_data,"y",Device_public::gps_information.GPS_Latitude);
    if(Device_public::wodeep<0)
    {
        cJSON_AddNumberToObject(event_data, "depthSymbol",1);//作业深度符号 1负数 2正数
        Device_public::wodeep = 0-Device_public::wodeep;
        cJSON_AddNumberToObject(event_data, "depthH", Device_public::wodeep/10);//作业深度 单位：cm 十位
        cJSON_AddNumberToObject(event_data, "depthL", Device_public::wodeep%10);//作业深度 单位：cm 个位
    }
    else
    {
        cJSON_AddNumberToObject(event_data, "depthSymbol",2);//作业深度符号 1负数 2正数
        cJSON_AddNumberToObject(event_data, "depthH", Device_public::wodeep/10);//作业深度 单位：cm 十位
        cJSON_AddNumberToObject(event_data, "depthL",Device_public::wodeep%10);//作业深度 单位：cm 个位
    }
    cJSON_AddNumberToObject(event_data, "speedB",Device_public::speed/100);//单位：km/h 百位
    cJSON_AddNumberToObject(event_data, "speedS",Device_public::speed/10%10);//单位：km/h十位
    cJSON_AddNumberToObject(event_data, "speedG",Device_public::speed%10);//单位：km/h个位
    cJSON_AddNumberToObject(event_data, "distance", 0);
    cJSON_AddNumberToObject(event_data, "status", 0);
    cJSON_AddNumberToObject(event_data, "systemIdH", Device_public::Motype/10);//作业模式十位
    cJSON_AddNumberToObject(event_data, "systemIdL", Device_public::Motype%10);//作业模式个位
    cJSON_AddNumberToObject(event_data, "binding", Device_public::Balingoutputnumber);//打捆值
    cJSON_AddNumberToObject(event_data, "start_status", 1);
    //  字节数组转化
    QByteArray pub_payload_string(cJSON_Print(event_data));

    //qDebug()<< pub_payload_string.data();
    //qDebug()<< pub_payload_string.toBase64().data();

    cJSON_Delete(event_data);
    res =   aiot_mqtt_pub(Device_public::mqtt_handle, pub_topic_char.data(), (uint8_t *)pub_payload_string.toBase64().data(),pub_payload_string.toBase64().size(), 0);
    if (res < 0) {
         printf("aiot_mqtt_sub failed, res: -0x%04X\n", -res);
    }
}


