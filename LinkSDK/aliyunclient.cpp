#include "aliyunclient.h"


AliyunClient::AliyunClient(QObject *parent):
    QObject(parent)
{

    mqtt_thread_pro = new Mqtt_thread_process;
    mqtt_thread_rev = new Mqtt_thread_recv;

    int32_t res = STATE_SUCCESS;
    char*  product_key;
    QByteArray ba1 = Device_public::ProductKey.toLatin1(); //
    product_key=ba1.data();
    char*  device_name;
    QByteArray ba2 = Device_public::device_number.toLatin1(); //
    device_name=ba2.data();
    char*  device_secret;
    QByteArray ba3 = Device_public::DeviceSecret.toLatin1(); //
    device_secret=ba3.data();
    qDebug()<<"ProductKey:"<< Device_public::ProductKey <<"DeviceName:" <<Device_public::device_number <<"DeviceSecret:"<< Device_public::DeviceSecret;
    res = demo_mqtt_start(&Device_public::mqtt_handle,product_key,device_name,device_secret);

    if(res<0){
         printf("demo_mqtt_start failed\n");
    }else{
        //云平台连接失败
        emit tts_message(1,"4E915E7353F08FDE63A559318D25");
    }

    qDebug() << "demo_mqtt_start succeed\n";


//    //定时任务
//    timer=new QTimer(this);
//    connect(timer,SIGNAL(timeout()),this,SLOT(macian_mqtt_pub_time()));
//    timer->start(5000);

}

void AliyunClient::macian_mqtt_pub_time()
{

}


/* MQTT默认消息处理回调, 当SDK从服务器收到MQTT消息时, 且无对应用户回调处理时被调用 */
void AliyunClient::demo_mqtt_default_recv_handler(void *handle, const aiot_mqtt_recv_t *packet, void *userdata)
{
    cJSON* root = NULL;
    cJSON* itemName = NULL;
    QString Original_value;

    switch (packet->type) {
        case AIOT_MQTTRECV_HEARTBEAT_RESPONSE: {
            printf("heartbeat response\n");
            /* TODO: 处理服务器对心跳的回应, 一般不处理 */
        }
        break;

        case AIOT_MQTTRECV_SUB_ACK: {
            printf("suback, res: -0x%04X, packet id: %d, max qos: %d\n",
                   -packet->data.sub_ack.res, packet->data.sub_ack.packet_id, packet->data.sub_ack.max_qos);
             /* TODO: 处理服务器对订阅请求的回应, 一般不处理 */
        }
        break;

        case AIOT_MQTTRECV_PUB: {
            //printf("pub, qos: %d, topic: %.*s\n", packet->data.pub.qos, packet->data.pub.topic_len, packet->data.pub.topic);
            //printf("pub, payload: %.*s\n", packet->data.pub.payload_len, packet->data.pub.payload);

            QByteArray payload_bytearry((const char*)packet->data.pub.payload);
            QByteArray payload_bytearry_decode = payload_bytearry.fromBase64(payload_bytearry);

            if(strstr((char*)packet->data.pub.topic, "set_mode")){
               //从缓冲区中解析json结构
               root = cJSON_Parse(payload_bytearry_decode.data());
               qDebug()<<payload_bytearry_decode.data();
               if(root != NULL){
                   //从json包中获取deviceNumber
                   itemName = cJSON_GetObjectItem(root, "deviceNumber");//设备号
                   //从json包中获取deviceNumber 比对deviceNumber正确
                   QString itemvalue_string = itemName->valuestring;
                   qDebug()<<"itemName->valuestring"<<itemvalue_string;
                   qDebug()<<"Device_public::device_number"<<Device_public::device_number;
                   //if(itemvalue_string.contains(Device_public::device_number)){
                        itemName = cJSON_GetObjectItem(root, "mode");

                        //从json包中获取deviceNumber 比对deviceNumber正确 再从json包中获取作业模式
                        if(itemName->valueint!=0){
                            Device_public::Motype = itemName->valueint;
                            qDebug()<<"PUB:Device_public::Motype"<<Device_public::Motype;

                            IniParse ini(AMIOT_CONFIG);
                            ini.Set("Device.Motype",Device_public::Motype);
                            ini.SaveFile();
                        }
                   //}
                   cJSON_Delete(root);
               }
            }

            //校准包
            else if(strstr((char*)packet->data.pub.topic, "calibrated")){
                //从缓冲区中解析json结构
                root = cJSON_Parse(payload_bytearry_decode.data());
                if (root != NULL){
                    itemName = cJSON_GetObjectItem(root, "deviceNumber");//设备号
                    if(strstr((char*)Device_public::device_number.toLatin1().data(),(char*)itemName->valuestring)){
                        //10分钟内可以校准；超出时间不校准||开始校准计时
                        if(Device_public::MQTT_Calibration_Time_flag == 1){
                            itemName = cJSON_GetObjectItem(root, "type");
                            //0 临界点校准   1 声音校准  2 工作点校准
                            if(strstr((char*)itemName->valuestring, "0")){   //临界点校准
                                Device_public::Fvalue = 1;//校准标志位
                                itemName = cJSON_GetObjectItem(root, "value");
                                Original_value = QString(QLatin1String(itemName->valuestring));
                                //临界点直接设置臂长
                                Device_public::Cvalue.length =  Original_value.toUInt();
                                //临界点主要记录原始角度，在工作校准时利用工作时角度和设置工作点数据计算出臂长
                                //深翻作业需要两个深度传感器计算，需要记录两个传感器值
                                if(Device_public::Motype == 3){
                                    Device_public::Cvalue.angle1 = 1;
                                    Device_public::Cvalue.angle2 = 1;
                                }
                                else{
                                    Device_public::Cvalue.angle1 = 1;
                                    Device_public::Cvalue.angle2 = 0;
                                }
                                IniParse ini(AMIOT_CONFIG);
                                ini.Set("Device.CvalueLength",(int)Device_public::Cvalue.length);
                                ini.Set("Device.CvalueAngle1",(int)Device_public::Cvalue.angle1);
                                ini.Set("Device.CvalueAngle2",(int)Device_public::Cvalue.angle2);
                                ini.SaveFile();
                            }
                            else if(strstr((char*)itemName->valuestring, "1")){//声音校准
                                Device_public::Fvalue = 1;//校准标志位
                                itemName = cJSON_GetObjectItem(root, "value");
                                Device_public::Vvalue = QString(QLatin1String(itemName->valuestring));
                                IniParse ini(AMIOT_CONFIG);
                                ini.Set("Device.Vvalue",Device_public::Vvalue.toStdString());
                                ini.SaveFile();
                            }
                            else if(strstr((char*)itemName->valuestring, "2")){//工作点校准类型
                                float CurrentAngle = 0.0;
                                float CriticalAngle = 0.0;
                                Device_public::Fvalue = 1;//校准标志位
                                itemName = cJSON_GetObjectItem(root, "value");
                                Device_public::Wvalue.length = QString(QLatin1String(itemName->valuestring)).toUInt();
                                Original_value = QString(QLatin1String(itemName->valuestring));

                                CriticalAngle = sin(Device_public::Cvalue.angle1)*(float)100;//原角度
                                CurrentAngle = sin(1)*(float)100;//现角度

                                Device_public::Wvalue.length =  ((float)Original_value.toUInt()*100)/(CurrentAngle-CriticalAngle);

                                IniParse ini(AMIOT_CONFIG);
                                ini.Set("Device.Wvalue",(int)Device_public::Wvalue.length);
                                ini.SaveFile();
                            }
                            else
                            {
                                 Device_public::Fvalue = 0;//校准标志位
                                 printf("\r\n clibrated error \n");
                            }
                        }
                        else{
                             printf("\r\n clibrated over 10 min\n");
                        }
                    }
                    cJSON_Delete(root);
                }
            }
            //远程升级包
            else if(strstr((char*)packet->data.pub.topic, "update_data")){
                //从缓冲区中解析json结构
                root = cJSON_Parse(payload_bytearry_decode.data());
                if (root != NULL){
                    itemName = cJSON_GetObjectItem(root, "deviceNumber");//设备号
                    if(strstr((char*)Device_public::device_number.toLatin1().data(),(char*)itemName->valuestring)){

                        itemName = cJSON_GetObjectItem(root, "file_md5");
                        Device_public::file_MD5 = itemName->valuestring;

                        IniParse ini(AMIOT_CONFIG);
                        ini.Set("Version.file_MD5",Device_public::file_MD5.toStdString());

                        itemName = cJSON_GetObjectItem(root, "fileUrl");
                        Device_public::file_Url = itemName->valuestring;
                        ini.Set("Version.file_Url",Device_public::file_Url.toStdString());

                        itemName = cJSON_GetObjectItem(root, "version");

                        Device_public::new_Version = itemName->valuestring;
                        if(Device_public::new_Version.compare(Device_public::current_Version) == 0){
                            //当前为最新版本

                            return ;
                        }
                        else if(Device_public::new_Version.compare(Device_public::current_Version) < 0){
                            //升级版本是旧版本
                            Device_public::Update_flag = 1;
                        }
                        else if(Device_public::new_Version.compare(Device_public::current_Version) > 0){
                            //升级版本是新版本
                            Device_public::Update_flag = 1;
                        }
                        ini.Set("Version.new_Version",Device_public::new_Version.toStdString());
                        ini.SaveFile();
                    }
                    cJSON_Delete(root);
                }
            }
            //停止模式接收消息
            else if(strstr((char*)packet->data.pub.topic, "send_workstatus")){
                //从缓冲区中解析json结构
                root = cJSON_Parse(payload_bytearry_decode.data());
                if (root != NULL){
                    itemName = cJSON_GetObjectItem(root, "deviceNumber");//设备号
                    if(strstr((char*)Device_public::device_number.toLatin1().data(),(char*)itemName->valuestring)){
                        itemName = cJSON_GetObjectItem(root, "type");
                        Device_public::Stop_workmode = atoi(itemName->valuestring);//校准类型

                        IniParse ini(AMIOT_CONFIG);
                        ini.Set("Device.Stop_workmode",Device_public::Stop_workmode);
                        ini.SaveFile();
                    }
                    cJSON_Delete(root);
                }
            }
            //停止模式接收消息
            else if(strstr((char*)packet->data.pub.topic, "repeat_info")){
                //从缓冲区中解析json结构
                root = cJSON_Parse(payload_bytearry_decode.data());
                if (root != NULL){
                    itemName = cJSON_GetObjectItem(root, "deviceNumber");//设备号
                    if(strstr((char*)Device_public::device_number.toLatin1().data(),(char*)itemName->valuestring)){
                        itemName = cJSON_GetObjectItem(root, "is_repeat");
                        Device_public::Repeak_work = itemName->valueint;//校准类型

                        IniParse ini(AMIOT_CONFIG);
                        ini.Set("Device.Repeak_work",Device_public::Stop_workmode);
                        ini.SaveFile();
                    }
                    cJSON_Delete(root);
                }
            }
            //错误消息
            else if(strstr((char*)packet->data.pub.topic, "/ext/error")){
                    if(strstr((char*)packet->data.pub.topic, "no active session")){
                        printf("\r\n device is unregistered \r\n");
                    }

            }
            /* TODO: 处理服务器下发的业务报文 */
        }
        break;

        case AIOT_MQTTRECV_PUB_ACK: {
            printf("puback, packet id: %d\n", packet->data.pub_ack.packet_id);
            /* TODO: 处理服务器对QoS1上报消息的回应, 一般不处理 */
        }
        break;

        default: {

        }
    }
}

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
/* MQTT事件回调函数, 当网络连接/重连/断开时被触发, 事件定义见core/aiot_mqtt_api.h */
void AliyunClient::demo_mqtt_event_handler(void *handle, const aiot_mqtt_event_t *event, void *userdata)
{
    switch (event->type) {
        /* SDK因为用户调用了aiot_mqtt_connect()接口, 与mqtt服务器建立连接已成功 */
        case AIOT_MQTTEVT_CONNECT: {
            printf("AIOT_MQTTEVT_CONNECT\n");
            /* TODO: 处理SDK建连成功, 不可以在这里调用耗时较长的阻塞函数 */
        }
        break;

        /* SDK因为网络状况被动断连后, 自动发起重连已成功 */
        case AIOT_MQTTEVT_RECONNECT: {
            printf("AIOT_MQTTEVT_RECONNECT\n");
            /* TODO: 处理SDK重连成功, 不可以在这里调用耗时较长的阻塞函数 */
        }
        break;

        /* SDK因为网络的状况而被动断开了连接, network是底层读写失败, heartbeat是没有按预期得到服务端心跳应答 */
        case AIOT_MQTTEVT_DISCONNECT: {
//            char *cause = (event->data.disconnect == AIOT_MQTTDISCONNEVT_NETWORK_DISCONNECT) ? ("network disconnect") :("heartbeat disconnect");
//            printf("AIOT_MQTTEVT_DISCONNECT: %s\n", cause);
            /* TODO: 处理SDK被动断连, 不可以在这里调用耗时较长的阻塞函数 */
        }
        break;

        default: {
        }
    }
}

/* TODO: 如果要关闭日志, 就把这个函数实现为空, 如果要减少日志, 可根据code选择不打印
 *
 * 例如: [1581501698.455][LK-0309] pub: /ext/ntp/a13FN5TplKq/ntp_basic_demo/request
 *
 * 上面这条日志的code就是0309(十六进制), code值的定义见core/aiot_state_api.h
 *
 */

/* 日志回调函数, SDK的日志会从这里输出 */
//int32_t* AliyunClient::demo_state_logcb(int32_t code, char *message)
//{
//    printf("%d,%s", code,message);
//    return 0;
//}

int32_t AliyunClient::demo_mqtt_start(void **handle,char *ProductKey,char *DeviceName,char *DeviceSecret)
{
    int32_t     res = STATE_SUCCESS;
    void       *mqtt_handle = NULL;
    void       *ntp_handle = NULL;
    char       *url = "iot-as-mqtt.cn-shanghai.aliyuncs.com"; /* 阿里云平台上海站点的域名后缀 */
    char        host[100] = {0}; /* 用这个数组拼接设备连接的云平台站点全地址, 规则是 ${productKey}.iot-as-mqtt.cn-shanghai.aliyuncs.com */
    uint16_t    port = 443;      /* 无论设备是否使用TLS连接阿里云平台, 目的端口都是443 */
    aiot_sysdep_network_cred_t cred; /* 安全凭据结构体, 如果要用TLS, 这个结构体中配置CA证书等参数 */

    /* 配置SDK的底层依赖 */
    aiot_sysdep_set_portfile(&g_aiot_sysdep_portfile);
    /* 配置SDK的日志输出 */
    //aiot_state_set_logcb(demo_state_logcb);

    /* 创建SDK的安全凭据, 用于建立TLS连接 */
    memset(&cred, 0, sizeof(aiot_sysdep_network_cred_t));
    cred.option = AIOT_SYSDEP_NETWORK_CRED_SVRCERT_CA;  /* 使用RSA证书校验MQTT服务端 */
    cred.max_tls_fragment = 16384; /* 最大的分片长度为16K, 其它可选值还有4K, 2K, 1K, 0.5K */
    cred.sni_enabled = 1;                               /* TLS建连时, 支持Server Name Indicator */
    cred.x509_server_cert = ali_ca_cert;                 /* 用来验证MQTT服务端的RSA根证书 */
    cred.x509_server_cert_len = strlen(ali_ca_cert);     /* 用来验证MQTT服务端的RSA根证书长度 */

    /* 创建1个MQTT客户端实例并内部初始化默认参数 */
    mqtt_handle = aiot_mqtt_init();
    if (mqtt_handle == NULL) {
        printf("aiot_mqtt_init failed\n");
        return -1;
    }

    /* TODO: 如果以下代码不被注释, 则例程会用TCP而不是TLS连接云平台 */
    /*
    {
        memset(&cred, 0, sizeof(aiot_sysdep_network_cred_t));
        cred.option = AIOT_SYSDEP_NETWORK_CRED_NONE;
    }
    */

    snprintf(host, 100, "%s.%s", ProductKey, url);
    /* 配置MQTT服务器地址 */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_HOST, (void *)host);
    /* 配置MQTT服务器端口 */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_PORT, (void *)&port);
    /* 配置设备productKey */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_PRODUCT_KEY, (void *)ProductKey);
    /* 配置设备deviceName */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_DEVICE_NAME, (void *)DeviceName);
    /* 配置设备deviceSecret */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_DEVICE_SECRET, (void *)DeviceSecret);
    /* 配置网络连接的安全凭据, 上面已经创建好了 */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_NETWORK_CRED, (void *)&cred);
    /* 配置MQTT默认消息接收回调函数 */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_RECV_HANDLER, (void *)demo_mqtt_default_recv_handler);
    /* 配置MQTT事件回调函数 */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_EVENT_HANDLER, (void *)demo_mqtt_event_handler);

    /* 与服务器建立MQTT连接 */
    res = aiot_mqtt_connect(mqtt_handle);
    if (res < STATE_SUCCESS) {
        /* 尝试建立连接失败, 销毁MQTT实例, 回收资源 */
        aiot_mqtt_deinit(&mqtt_handle);
        printf("aiot_mqtt_connect failed: -0x%04X\n", -res);
        return res;
    }

    /* MQTT 订阅topic功能示例, 请根据自己的业务需求进行使用 */
    {
        //订阅获取模式
        char sub_topic[120]={0};
        snprintf(sub_topic,120,"/%s/%s/user/set_mode",ProductKey,DeviceName);
        res = aiot_mqtt_sub(mqtt_handle, sub_topic, NULL, 1, NULL);
        if (res < 0) {
            printf("aiot_mqtt_sub failed, res: -0x%04X\n", -res);
            return res;
        }
        memset(sub_topic,0,sizeof(sub_topic));
        //订阅远程升级
        snprintf(sub_topic,120,"/%s/%s/user/update_data",ProductKey,DeviceName);
        res = aiot_mqtt_sub(mqtt_handle, sub_topic, NULL, 1, NULL);
        if (res < 0) {
            printf("aiot_mqtt_sub failed, res: -0x%04X\n", -res);
            return res;
        }
        memset(sub_topic,0,sizeof(sub_topic));
        //订阅重复通知
        snprintf(sub_topic,120,"/%s/%s/user/repeat_info",ProductKey,DeviceName);
        res = aiot_mqtt_sub(mqtt_handle, sub_topic, NULL, 1, NULL);
        if (res < 0) {
            printf("aiot_mqtt_sub failed, res: -0x%04X\n", -res);
            return res;
        }
        memset(sub_topic,0,sizeof(sub_topic));
         //订阅校准
        snprintf(sub_topic,120,"/%s/%s/user/calibrated",ProductKey,DeviceName);
        res = aiot_mqtt_sub(mqtt_handle, sub_topic, NULL, 1, NULL);
        if (res < 0) {
            printf("aiot_mqtt_sub failed, res: -0x%04X\n", -res);
            return res;
        }
        memset(sub_topic,0,sizeof(sub_topic));
        //订阅发送工作状态  1停机 2启用
        snprintf(sub_topic,120,"/%s/%s/user/send_workstatus",ProductKey,DeviceName);
        res = aiot_mqtt_sub(mqtt_handle, sub_topic, NULL, 1, NULL);
        if (res < 0) {
            printf("aiot_mqtt_sub failed, res: -0x%04X\n", -res);
            return res;
        }
    }
    /* MQTT 发布消息功能示例, 请根据自己的业务需求进行使用 */
//        char *pub_topic = "/a1LOqD9O9II/20200701001/user/work_data";
//        char *pub_payload = "111";
//        res = aiot_mqtt_pub(mqtt_handle, pub_topic, (uint8_t *)pub_payload, (uint32_t)strlen(pub_payload), 0);
//        if (res < 0) {
//            printf("aiot_mqtt_sub failed, res: -0x%04X\n", -res);
//            return -1;
//        }


    /* 创建一个单独的线程, 专用于执行aiot_mqtt_process, 它会自动发送心跳保活, 以及重发QoS1的未应答报文 */
    mqtt_thread_pro->start();
//    g_mqtt_process_thread_running = 1;
//    QtConcurrent::run(this,demo_mqtt_process_thread);
//   res = pthread_create(&g_mqtt_process_thread, NULL, demo_mqtt_process_thread, mqtt_handle);
//    if (res < 0) {
//        printf("pthread_create demo_mqtt_process_thread failed: %d\n", res);
//        aiot_mqtt_deinit(&mqtt_handle);
//        return -1;
//    }

    /* 创建一个单独的线程用于执行aiot_mqtt_recv, 它会循环收取服务器下发的MQTT消息, 并在断线时自动重连 */
    mqtt_thread_rev->start();
//    g_mqtt_recv_thread_running = 1;
//    res = pthread_create(&g_mqtt_recv_thread, NULL, demo_mqtt_recv_thread, mqtt_handle);
//    if (res < 0) {
//        printf("pthread_create demo_mqtt_recv_thread failed: %d\n", res);
//        g_mqtt_process_thread_running = 0;
//        pthread_join(g_mqtt_process_thread, NULL);
//        aiot_mqtt_deinit(&mqtt_handle);
//        return -1;
//    }

    int8_t      time_zone = 8;
    //mqtt_handle = *handle_mqtt;
    //ntp_handle = *handle_ntp;

    /* 创建1个ntp客户端实例并内部初始化默认参数 */
    ntp_handle = aiot_ntp_init();
    if (ntp_handle == NULL) {
        demo_mqtt_stop(&mqtt_handle);
        printf("aiot_ntp_init failed\n");
        return res;
    }

    res = aiot_ntp_setopt(ntp_handle, AIOT_NTPOPT_MQTT_HANDLE, mqtt_handle);
    if (res < STATE_SUCCESS) {
        printf("aiot_ntp_setopt AIOT_NTPOPT_MQTT_HANDLE failed, res: -0x%04X\n", -res);
        aiot_ntp_deinit(&ntp_handle);
        demo_mqtt_stop(&mqtt_handle);
        return res;
    }

    res = aiot_ntp_setopt(ntp_handle, AIOT_NTPOPT_TIME_ZONE, (int8_t *)&time_zone);
    if (res < STATE_SUCCESS) {
        printf("aiot_ntp_setopt AIOT_NTPOPT_TIME_ZONE failed, res: -0x%04X\n", -res);
        aiot_ntp_deinit(&ntp_handle);
        demo_mqtt_stop(&mqtt_handle);
        return res;
    }

    /* TODO: NTP消息回应从云端到达设备时, 会进入此处设置的回调函数 */
    res = aiot_ntp_setopt(ntp_handle, AIOT_NTPOPT_RECV_HANDLER, (void *)demo_ntp_recv_handler);
    if (res < STATE_SUCCESS) {
        printf("aiot_ntp_setopt AIOT_NTPOPT_RECV_HANDLER failed, res: -0x%04X\n", -res);
        aiot_ntp_deinit(&ntp_handle);
        demo_mqtt_stop(&mqtt_handle);
        return res;
    }

    res = aiot_ntp_setopt(ntp_handle, AIOT_NTPOPT_EVENT_HANDLER, (void *)demo_ntp_event_handler);
    if (res < STATE_SUCCESS) {
        printf("aiot_ntp_setopt AIOT_NTPOPT_EVENT_HANDLER failed, res: -0x%04X\n", -res);
        aiot_ntp_deinit(&ntp_handle);
        demo_mqtt_stop(&mqtt_handle);
        return res;
    }
    res = aiot_ntp_send_request(ntp_handle);
    if (res < STATE_SUCCESS) {
          aiot_ntp_deinit(&ntp_handle);
          demo_mqtt_stop(&mqtt_handle);
    }

    *handle = mqtt_handle;
    return STATE_SUCCESS;
}
int32_t AliyunClient::demo_mqtt_stop(void **handle)
{
    int32_t res = STATE_SUCCESS;
    void *mqtt_handle = NULL;
    mqtt_handle = *handle;

    Device_public::g_mqtt_process_thread_running = 0;
    Device_public::g_mqtt_recv_thread_running = 0;
    mqtt_thread_rev->quit();
    mqtt_thread_pro->quit();

    /* 断开MQTT连接 */
    res = aiot_mqtt_disconnect(mqtt_handle);
    if (res < STATE_SUCCESS) {
        aiot_mqtt_deinit(&mqtt_handle);
        printf("aiot_mqtt_disconnect failed: -0x%04X\n", -res);
        return res;
    }

    /* 销毁MQTT实例 */
    res = aiot_mqtt_deinit(&mqtt_handle);
    if (res < STATE_SUCCESS) {
        printf("aiot_mqtt_deinit failed: -0x%04X\n", -res);
        return res;
    }

    return STATE_SUCCESS;
}



int32_t AliyunClient::macian_mqtt_start(void *handle,char *ProductKey,char *DeviceName,char *DeviceSecret)
{
    int32_t res = STATE_SUCCESS;
    res = demo_mqtt_start(&handle,ProductKey,DeviceName,DeviceSecret);

    return res;
}

int32_t AliyunClient::macian_mqtt_stop(void *handle)
{
     int32_t res = STATE_SUCCESS;
     res = demo_mqtt_stop(&handle);

     return res;
}

/* MQTT 发布消息功能*/
int32_t AliyunClient::macian_mqtt_pub(void *handle_mqtt,char *topic_pub,char *payload_pub)
{
    int32_t res = STATE_SUCCESS;
    res = aiot_mqtt_pub(handle_mqtt, topic_pub, (uint8_t *)payload_pub, (uint32_t)strlen(payload_pub), 0);
    if (res < 0) {
          printf("aiot_mqtt_sub failed, res: -0x%04X\n", -res);
          return res;
    }
     return STATE_SUCCESS;
}

/* 发送NTP查询请求给云平台 */
int32_t AliyunClient::macian_ntp_send_request(void * handle_mqtt,void *handle_ntp)
{
    int32_t res = STATE_SUCCESS;
    res = aiot_ntp_send_request(handle_ntp);
    if (res < STATE_SUCCESS) {
        aiot_ntp_deinit(&handle_ntp);
        demo_mqtt_stop(&handle_mqtt);
        return res;
    }
    return STATE_SUCCESS;
}

int32_t AliyunClient::macian_ntp_init(void **handle_mqtt, void **handle_ntp)
{
    int32_t     res = STATE_SUCCESS;
    void *mqtt_handle = NULL;
    void *ntp_handle = NULL;
    /*
     * 这里用中国所在的东8区演示功能, 因此例程运行时打印的是北京时间
     *
     * TODO: 若是其它时区, 可做相应调整, 如西3区则把8改为-3, time_zone的合理取值是-12到+12的整数
     *
     */
    int8_t      time_zone = 8;
//    mqtt_handle = *handle_mqtt;
//    ntp_handle = *handle_ntp;

    /* 创建1个ntp客户端实例并内部初始化默认参数 */
    ntp_handle = aiot_ntp_init();
    if (ntp_handle == NULL) {
        demo_mqtt_stop(&mqtt_handle);
        printf("aiot_ntp_init failed\n");
        return res;
    }

    res = aiot_ntp_setopt(ntp_handle, AIOT_NTPOPT_MQTT_HANDLE, mqtt_handle);
    if (res < STATE_SUCCESS) {
        printf("aiot_ntp_setopt AIOT_NTPOPT_MQTT_HANDLE failed, res: -0x%04X\n", -res);
        aiot_ntp_deinit(&ntp_handle);
        demo_mqtt_stop(&mqtt_handle);
        return res;
    }

    res = aiot_ntp_setopt(ntp_handle, AIOT_NTPOPT_TIME_ZONE, (int8_t *)&time_zone);
    if (res < STATE_SUCCESS) {
        printf("aiot_ntp_setopt AIOT_NTPOPT_TIME_ZONE failed, res: -0x%04X\n", -res);
        aiot_ntp_deinit(&ntp_handle);
        demo_mqtt_stop(&mqtt_handle);
        return res;
    }

    /* TODO: NTP消息回应从云端到达设备时, 会进入此处设置的回调函数 */
    res = aiot_ntp_setopt(ntp_handle, AIOT_NTPOPT_RECV_HANDLER, (void *)demo_ntp_recv_handler);
    if (res < STATE_SUCCESS) {
        printf("aiot_ntp_setopt AIOT_NTPOPT_RECV_HANDLER failed, res: -0x%04X\n", -res);
        aiot_ntp_deinit(&ntp_handle);
        demo_mqtt_stop(&mqtt_handle);
        return res;
    }

    res = aiot_ntp_setopt(ntp_handle, AIOT_NTPOPT_EVENT_HANDLER, (void *)demo_ntp_event_handler);
    if (res < STATE_SUCCESS) {
        printf("aiot_ntp_setopt AIOT_NTPOPT_EVENT_HANDLER failed, res: -0x%04X\n", -res);
        aiot_ntp_deinit(&ntp_handle);
        demo_mqtt_stop(&mqtt_handle);
        return res;
    }
    res = aiot_ntp_send_request(ntp_handle);
    if (res < STATE_SUCCESS) {
          aiot_ntp_deinit(&ntp_handle);
          demo_mqtt_stop(&mqtt_handle);
    }

    *handle_mqtt = mqtt_handle;
    *handle_ntp = ntp_handle;
  return STATE_SUCCESS;
}

AliyunClient::~AliyunClient()
{
    int32_t res = STATE_SUCCESS;

    mqtt_thread_rev->quit();
    mqtt_thread_pro->quit();
    /* 销毁NTP实例, 一般不会运行到这里 */
    res = aiot_ntp_deinit(&Device_public::ntp_handle);
    if (res < STATE_SUCCESS) {
        demo_mqtt_stop(&Device_public::mqtt_handle);
        printf("aiot_ntp_deinit failed: -0x%04X\n", -res);
    }
    /* 销毁MQTT实例, 退出线程, 一般不会运行到这里 */
    res = macian_mqtt_stop(&Device_public::mqtt_handle);
    if (res < 0) {
        printf("demo_start_stop failed\n");

    }
}
