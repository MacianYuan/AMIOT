#include "device_public.h"


Device_public::Device_public()
{

}

//QSqlDatabase Device_public::db;

//unsigned int Device_public::SystemFlow = 0;
//bool Device_public::SystemFlow_flag = true;

bool Device_public::state_GPS = false;
unsigned int Device_public::state_Sersor = 0; //0 正在检测    1 在线  2 脱机
double Device_public::state_tf_total;
double Device_public::state_tf_use;


GPS_information Device_public::gps_information;

HttpEvent Device_public::Http_Event;

QextSerialPort * Device_public::key_port = NULL;
QextSerialPort * Device_public::tts_port = NULL;
QextSerialPort * Device_public::gps_port = NULL;
QextSerialPort * Device_public::sensor_port = NULL;

QDateTime Device_public::timeString;


QString Device_public::ProductKey;  //产品主键
QByteArray Device_public::DeviceName;  //机具号
QString Device_public::DeviceSecret;//产品秘钥


//作业模式&传感器相关
//打捆作业模式 8
uint8_t Device_public::Bunding8_WorkMode;//1输出打捆、深度  2输出深度  3输出旋转  4输出打捆、旋转  0不输出
//小麦模式 4 5 6 10 13 14
uint8_t Device_public::Wheat4_WorkMode;//1输出深度 2输出旋转 0不输出
//深翻作业 3
uint8_t Device_public::SpulchnianiePodglebia3_WorkMode;//1输出深度最大值  2输出深度1 3输出深度2  0不输出
//深松作业 1 2 9 11 12 15
uint8_t Device_public::Deep_sigh_homework1_WorkMode;
//机具传感器
bool Device_public::Internal_DN = 0; //外置机具号没有识别到，是否使用内置机具号
//其他数据传感器
bool Device_public::Sensor1_Online = 0;//深度传感器是否在线
bool Device_public::Sensor2_Online = 0;//旋转（深翻作业3深度）传感器是否在线
bool Device_public::Sensor3_Online = 0;//打捆传感器是否在线
bool Device_public::Sensor4_Online = 0;//播种传感器是否在线
float Device_public::sensor1_data;
float Device_public::sensor2_data;
float Device_public::sensor3_data;
float Device_public::sensor4_data;
float Device_public::sensor_oil_data;

//播种传感器信息
uint8_t Device_public::plant_distance;//株距
uint8_t Device_public::line_number;//行数
uint8_t Device_public::line_distance;//行距
uint8_t Device_public::seeds_number;//种子数


//工作数据相关
QString Device_public::eventDate_data;  //
QString Device_public::cmd_data;  // 1 mqtt数据传输开关
QString Device_public::type_data;//1 该条上报数据类型  1有效 2无效
QString Device_public::device_number;//设备号
uint8_t Device_public::binding_data;  //打捆数目
QString Device_public::data_time_data;  //
//QString Device_public::x_data;// 经度
//QString Device_public::y_data;  // 纬度
QString Device_public::depth_data;  //作业深度
QString Device_public::speed_data;//地面速率
QString Device_public::distance_data;  //0
QString Device_public::status_data;  //0
QString Device_public::systemId_data;// 作业模式
QString Device_public::start_status_data;//1


//QString Device_public::Now_version = "APP_V3.0.0.1"; //设备版本号


//uint32_t Device_public::Longitude;// 经度
//uint32_t Device_public::Latitude;// 纬度
int16_t  Device_public::wodeep;//  工作深度
uint16_t Device_public::speed;// 地面速率

uint8_t  Device_public::Balingoutputnumber;//打捆数目

//红外解码
int  Device_public::filp ;

//mqtt相关
QString Device_public::HttpUrl;
//客户端处理
void *Device_public::mqtt_handle;
void *Device_public::ntp_handle;
int8_t Device_public::g_mqtt_process_thread_running;
uint8_t Device_public::g_mqtt_recv_thread_running;
//通讯处理
bool Device_public::MQTT_Calibration_Time_flag;
//json包解析
int  Device_public::Motype;//作业模式
//校准值

CP_calibration  Device_public::Cvalue;//临界点校准值
QString  Device_public::Vvalue;//声音校准值
WP_calibration  Device_public::Wvalue;//工作点校准值
bool  Device_public::Fvalue;//校准标志


QString  Device_public::file_MD5;//文件MD5
QString  Device_public::file_Url;//下载地址
QString  Device_public::new_Version;//版本号
//QString  Device_public::old_Version;//版本号
QString  Device_public::current_Version;//版本号
uint8_t  Device_public::Stop_workmode;//停止模式
uint8_t  Device_public::Repeak_work;//重复作业
uint8_t  Device_public::Update_flag;//升级标志    1： 等待升级
