/*
 * 这个例程适用于`Linux`这类支持pthread的POSIX设备, 它演示了用SDK配置MQTT参数并建立连接, 之后创建2个线程
 *
 * + 一个线程用于保活长连接
 * + 一个线程用于接收消息, 并在有消息到达时进入默认的数据回调, 在连接状态变化时进入事件回调
 *
 * 接着在MQTT连接上发送NTP查询请求, 如果云平台的回应报文到达, 从接收线程会调用NTP消息处理的回调函数, 把对时后的本地时间打印出来
 *
 * 需要用户关注或修改的部分, 已经用 TODO 在注释中标明
 *
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "LinkSDK/ntp_posix.h"
#include "rtc/hi_rtc.h"
#include "rtc/rtc_test.h"

/* 执行aiot_mqtt_process的线程, 包含心跳发送和QoS1消息重发 */
void *demo_mqtt_process_thread(int8_t  g_m_process,void * args)
{
    int32_t res = STATE_SUCCESS;

    while (g_m_process) {
        res = aiot_mqtt_process(args);
        if (res == STATE_USER_INPUT_EXEC_DISABLED) {
            break;
        }
        sleep(1);
    }
    return NULL;
}

/* 执行aiot_mqtt_recv的线程, 包含网络自动重连和从服务器收取MQTT消息 */
void *demo_mqtt_recv_thread(uint8_t g_m_recv,void *args)
{
    int32_t res = STATE_SUCCESS;

    while (g_m_recv) {
        res = aiot_mqtt_recv(args);
        if (res < STATE_SUCCESS) {
            if (res == STATE_USER_INPUT_EXEC_DISABLED) {
                break;
            }
            sleep(1);
        }
    }
    return NULL;
}

/* 事件处理回调,  */
void demo_ntp_event_handler(void *handle, const aiot_ntp_event_t *event, void *userdata)
{
    switch (event->type) {
        case AIOT_NTPEVT_INVALID_RESPONSE: {
            printf("AIOT_NTPEVT_INVALID_RESPONSE\n");
        }
        break;
        case AIOT_NTPEVT_INVALID_TIME_FORMAT: {
            printf("AIOT_NTPEVT_INVALID_TIME_FORMAT\n");
        }
        break;
        default: {

        }
    }
}

/* TODO: 数据处理回调, 当SDK从网络上收到ntp消息时被调用 */
void demo_ntp_recv_handler(void *handle, const aiot_ntp_recv_t *packet, void *userdata)
{
    char  timedata[100];
    switch (packet->type) {
        /* TODO: 结构体 aiot_ntp_recv_t{} 中包含当前时区下, 年月日时分秒的数值, 可在这里把它们解析储存起来 */
        case AIOT_NTPRECV_LOCAL_TIME: {
            sprintf(timedata,"date -s \"%d-%d-%d %d:%d:%d\"", packet->data.local_time.year,
                    packet->data.local_time.mon, packet->data.local_time.day, packet->data.local_time.hour, packet->data.local_time.min,
                    packet->data.local_time.sec);
            //printf("%s",timedata);
            system(timedata);
        }
        break;
        default: {
        }
    }
}

















//            printf("local time: %llu, %02d/%02d/%02d-%02d:%02d:%02d:%d\n",
//                   (long long unsigned int)packet->data.local_time.timestamp,
//                   packet->data.local_time.year,
//                   packet->data.local_time.mon, packet->data.local_time.day, packet->data.local_time.hour, packet->data.local_time.min,
//                   packet->data.local_time.sec, packet->data.local_time.msec);
//通过RTC驱动设置系统时间，存在问题 时间问题先通过系统时间命令初始化在通过QT获取当前时间实现
//             sprintf(timedata,"./test -s time \"%d-%d-%d %d:%d:%d\"", packet->data.local_time.year,
//                    packet->data.local_time.mon, packet->data.local_time.day, packet->data.local_time.hour, packet->data.local_time.min,
//                    packet->data.local_time.sec);

