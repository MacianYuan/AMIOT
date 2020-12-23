#ifndef ALIYUNCLIENT_H
#define ALIYUNCLIENT_H

#include <QObject>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include <QThread>
#include <QMutex>
#include <QTimer>
#include <QDebug>

#include "public/device_public.h"
#include "external/ali_ca_cert.h"
#include "portfiles/aiot_port/posix_port.h"
#include "LinkSDK/core/aiot_mqtt_api.h"
#include "LinkSDK/ntp_posix.h"
#include "LogicalProcessing/mqtt_logic_processing.h"

#include "LogicalProcessing/mqtt_thread_recv.h"
#include "LogicalProcessing/mqtt_thread_process.h"
#include "public/IniParse.h"
#include "public/version.h"
#include "public/httpgetfile.h"
#include <QStringList>


class AliyunClient : public QObject
{
    Q_OBJECT
public:
    explicit AliyunClient(QObject *parent = 0);
    int32_t macian_mqtt_pub(void *handle_mqtt,char *topic_pub,char *payload_pub);
    int32_t macian_ntp_send_request(void * handle_mqtt,void *handle_ntp);
    ~AliyunClient();

    //static pthread_t g_mqtt_process_thread;
    //static pthread_t g_mqtt_recv_thread;

signals:
    void tts_message(unsigned int num,QString str);
private:
    /* 日志回调函数, SDK的日志会从这里输出 */
    //int32_t* demo_state_logcb(int32_t code, char *message);

    /* 执行aiot_mqtt_process的线程, 包含心跳发送和QoS1消息重发 */
    //void *macian_mqtt_process_thread(void *args);
    /* 执行aiot_mqtt_recv的线程, 包含网络自动重连和从服务器收取MQTT消息 */
    //void *macian_mqtt_recv_thread(void *args);

    /* MQTT事件回调函数, 当网络连接/重连/断开时被触发, 事件定义见core/aiot_mqtt_api.h */
    //私有函数，外部无需引用 此处屏蔽
    static void demo_mqtt_event_handler(void *handle, const aiot_mqtt_event_t *event, void *userdata);
    /* MQTT默认消息处理回调, 当SDK从服务器收到MQTT消息时, 且无对应用户回调处理时被调用 */
    //私有函数，外部无需引用 此处屏蔽
    static void demo_mqtt_default_recv_handler(void *handle, const aiot_mqtt_recv_t *packet, void *userdata);

    int32_t macian_ntp_init(void **handle_mqtt, void **handle_ntp);
    int32_t macian_mqtt_start(void *handle,char *ProductKey,char *DeviceName,char *DeviceSecret);
    int32_t macian_mqtt_stop(void *handle);
    int32_t demo_mqtt_start(void **handle,char *ProductKey,char *DeviceName,char *DeviceSecret);
    int32_t demo_mqtt_stop(void **handle);
    /* 事件处理回调,  */
    //void macian_ntp_event_handler(void *handle, const aiot_ntp_event_t *event, void *userdata);
    /* TODO: 数据处理回调, 当SDK从网络上收到ntp消息时被调用 */
    //void macian_ntp_recv_handler(void *handle, const aiot_ntp_recv_t *packet, void *userdata);


    QTimer *timer;

    Mqtt_thread_recv *mqtt_thread_rev;
    Mqtt_thread_process *mqtt_thread_pro;



private slots:
    void macian_mqtt_pub_time();




public slots:
    
};


#endif // ALIYUNCLIENT_H
