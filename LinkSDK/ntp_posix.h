/**
 * @file aiot_http_api.h
 * @brief HTTP模块头文件, 提供用HTTP协议向阿里云物联网平台上报数据的能力
 * @date 2019-12-27
 *
 * @copyright Copyright (C) 2015-2018 Alibaba Group Holding Limited
 *
 */

#ifndef _NTP_POSIX_H_
#define _NTP_POSIX_H_

#if defined(__cplusplus)
extern "C" {
#endif

#include "LinkSDK/core/aiot_state_api.h"
#include "LinkSDK/core/aiot_sysdep_api.h"
#include "LinkSDK/core/aiot_mqtt_api.h"
#include "LinkSDK/components/ntp/aiot_ntp_api.h"
#include "LinkSDK/external/mbedtls/include/mbedtls/base64.h"
#include "cJSON.h"

//static int32_t demo_state_logcb(int32_t code, char *message);
/* MQTT事件回调函数, 当网络连接/重连/断开时被触发, 事件定义见core/aiot_mqtt_api.h */
//私有函数，外部无需引用 此处屏蔽
//void demo_mqtt_event_handler(void *handle, const aiot_mqtt_event_t *event, void *userdata);
/* MQTT默认消息处理回调, 当SDK从服务器收到MQTT消息时, 且无对应用户回调处理时被调用 */
//私有函数，外部无需引用 此处屏蔽
//void demo_mqtt_default_recv_handler(void *handle, const aiot_mqtt_recv_t *packet, void *userdata);

/* 执行aiot_mqtt_process的线程, 包含心跳发送和QoS1消息重发 */
//私有函数，外部无需引用 此处屏蔽
void *demo_mqtt_process_thread(int8_t g_m_process,void *args);
/* 执行aiot_mqtt_recv的线程, 包含网络自动重连和从服务器收取MQTT消息 */
//私有函数，外部无需引用 此处屏蔽
void *demo_mqtt_recv_thread(uint8_t g_m_recv,void *args);

///* MQTT初始化 配置SDK的底层依赖 配置SDK的日志输出 创建SDK的安全凭据, 用于建立TLS连接*/
//int32_t demo_mqtt_start(void **handle,char *ProductKey,char *DeviceName,char *DeviceSecret);
///* MQTT断开  断开MQTT连接 销毁MQTT实例*/
//int32_t demo_mqtt_stop(void **handle);
///* 事件处理回调,  */
void demo_ntp_event_handler(void *handle, const aiot_ntp_event_t *event, void *userdata);
///* 数据处理回调, 当SDK从网络上收到ntp消息时被调用 */
void demo_ntp_recv_handler(void *handle, const aiot_ntp_recv_t *packet, void *userdata);



#if defined(__cplusplus)
}
#endif

#endif /* #ifndef _AIOT_HTTP_API_H_ */

