/**
 * @file aiot_http_api.h
 * @brief HTTP模块头文件, 提供用HTTP协议向阿里云物联网平台上报数据的能力
 * @date 2019-12-27
 *
 * @copyright Copyright (C) 2015-2018 Alibaba Group Holding Limited
 *
 */

#ifndef _AIOT_HTTP_API_H_
#define _AIOT_HTTP_API_H_

#if defined(__cplusplus)
extern "C" {
#endif

#include "aiot_state_api.h"
#include "aiot_sysdep_api.h"
#include "aiot_mqtt_api.h"
#include "aiot_ntp_api.h"

int mqtt_main(int argc, char *argv[]);


#if defined(__cplusplus)
}
#endif

#endif /* #ifndef _AIOT_HTTP_API_H_ */

