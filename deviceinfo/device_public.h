#ifndef Device_public_H
#define Device_public_H

#include <QDebug>
#include "qextserial/serialcommunication.h"
#include "qextserial/qextserialport.h"
#include "stdint.h"
#include <QDateTime>

#include <QtSql/QtSql>
#include <QtSql/QSqlDatabase>

#define START_SYSTEM_FLOW_TTS           1
#define START_SYSTEM_FLOW_KEY           2
#define START_SYSTEM_FLOW_IAR           3
#define START_SYSTEM_FLOW_GPS           4
#define START_SYSTEM_FLOW_SENSOR        5
#define START_SYSTEM_FLOW_GETKEY        6
#define START_SYSTEM_FLOW_ALIYUN        7
#define START_SYSTEM_FLOW_UPDATE        8
#define START_SYSTEM_FLOW_UPLOAD        9

#define DIR_JPG_CACHE                   "/storage/snap"
#define DIR_264_CACHE                   "/storage/stream"

#define DIR_JPG_CACHE_TEMP              "/storage/snap_temp/"
#define DIR_264_CACHE_TEMP              "/storage/stream_temp/"

#define DATABASE_DRIVER                 "QSQLITE"
#define DATABASE_NAME                   "/tmp/sql.db"

//串口传感器  获取GPS信息
struct GPS_information{
    double GPS_Latitude;//纬度------------------------GPRMC
    QString GPS_Latitude_direction;
    double GPS_Longitude;//经度-----------------------GPRMC
    QString GPS_Longitude_direction;
    QString GPS_status;//GPS状态 A 有效定位  V无效定位----------GPRMC
    unsigned int GPS_num_satellites;//GPS可见卫星数----------------GPGGA
    QString GPS_speed;//GPS地面速率---------------------------GPVTG
};

struct HttpEvent{
    QString FileName;
    int32_t CameraId;
    int64_t FileSize;
    QString CreateTime;
    QString SyncTime;
    int32_t SyncFlag;
};

//阿里云和传感器获取临界点臂长和角度
struct CP_calibration{
    unsigned int length;
    float angle1;
    float angle2;
};
//阿里云和临界点数据获取工作点臂长和角度
struct WP_calibration{
    unsigned int length;
    float angle;
};

class Device_public
{
public:
    Device_public();
//    static QSqlDatabase db;
//    static unsigned int SystemFlow;
//    static bool SystemFlow_flag;

    static bool state_GPS;
    static unsigned int state_Sersor;
    static double state_tf_total;
    static double state_tf_use;

    static GPS_information gps_information;
    static HttpEvent Http_Event;
    static QextSerialPort *key_port;
    static QextSerialPort *tts_port;
    static QextSerialPort *gps_port;
    static QextSerialPort *sensor_port;

    static QDateTime timeString;

    //作业模式&传感器相关
    //打捆作业模式  8
    static uint8_t Bunding8_WorkMode;//1输出打捆、深度  2输出深度  3输出旋转  4输出打捆、旋转  0不输出
    //小麦模式 4 5 6 10 13 14
    static uint8_t Wheat4_WorkMode;//1输出深度 2输出旋转 0不输出
    //深翻作业 3
    static uint8_t SpulchnianiePodglebia3_WorkMode;//1输出深度最大值  2输出深度1  3输出深度2  0不输出
    //深松作业 1 2 9 11 12 15
    static uint8_t Deep_sigh_homework1_WorkMode;//1输出 无脱机

    //机具传感器
    static bool Internal_DN;//外置机具号没有识别到，是否使用内置机具号
    //其他数据传感器
    static bool Sensor1_Online;//深度传感器是否在线
    static bool Sensor2_Online;//旋转（深翻作业3深度）传感器是否在线
    static bool Sensor3_Online;//打捆传感器是否在线
    static bool Sensor4_Online;//播种传感器是否在线
    static float sensor1_data;
    static float sensor2_data;
    static float sensor3_data;
    static float sensor4_data;
    static float sensor_oil_data;
    //播种传感器信息
    static uint8_t plant_distance;//株距
    static uint8_t line_number;//行数
    static uint8_t line_distance;//行距
    static uint8_t seeds_number;//种子数



    static QString ProductKey;  //产品主键
    static QByteArray DeviceName;  //机具号
    static QString DeviceSecret;//设备密钥


//工作数据相关
    static QString eventDate_data;
    static QString cmd_data;  //
    static QString type_data;//
    static QString device_number;//设备号
    static uint8_t binding_data;  //打捆数目
    static QString data_time_data;  //
//    static QString x_data;//
//    static QString y_data;  //产品主键
    static QString depth_data;  //机具号
    static QString speed_data;//设备密钥
    static QString distance_data;  //产品主键
    static QString status_data;  //机具号
    static QString systemId_data;//设备密钥
    static QString start_status_data;//设备密钥




    static QString Now_version; //设备版本号

//    static QByteArray Longitude_bytearray;
//    static QByteArray Latitude_bytearray;
//    static QByteArray wodeep_bytearray;
//    static QByteArray speed_bytearray;
//    static QByteArray Motype_bytearray;
//    static QByteArray Balingoutputnumber_bytearray;

//    static uint32_t Longitude;
//    static uint32_t Latitude;
    static int16_t wodeep;
    static uint16_t speed;

    static uint8_t Balingoutputnumber;

    //红外解码
    static int  filp ;

    //mqtt相关
    static QString HttpUrl;
    //客户端处理
    static void *mqtt_handle ;
    static void *ntp_handle ;
    static int8_t g_mqtt_process_thread_running;
    static uint8_t g_mqtt_recv_thread_running;
    //通讯处理
    static bool MQTT_Calibration_Time_flag;

    //json包解析
    static int  Motype;//作业模式
    static CP_calibration  Cvalue;//临界点校准值
    static QString  Vvalue;//声音校准值
    static WP_calibration  Wvalue;//工作点校准值
    static bool     Fvalue;//校准标志
    static QString file_MD5;//文件MD5
    static QString file_Url;//下载地址
    static QString new_Version;//版本号
    //static QString old_Version;//版本号
    static QString current_Version;//当前版本号
    static uint8_t Stop_workmode;//停止模式
    static uint8_t Repeak_work;//重复作业
    //远程升级
    static uint8_t Update_flag;//升级标志    1： 等待升级
};

#endif // Device_public_H
