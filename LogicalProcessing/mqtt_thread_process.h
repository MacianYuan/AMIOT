#ifndef MQTT_THREAD_PROCESS_H
#define MQTT_THREAD_PROCESS_H

#include <QObject>
#include <QThread>
#include <QMutex>

#include "public/device_public.h"
#include "external/ali_ca_cert.h"
#include "portfiles/aiot_port/posix_port.h"
#include "LinkSDK/core/aiot_mqtt_api.h"
#include "LinkSDK/ntp_posix.h"

class Mqtt_thread_process : public QThread
{

public:
    Mqtt_thread_process();
    ~Mqtt_thread_process();
private:
    QMutex mutex;

protected:
    void run();
};

#endif // MQTT_THREAD_PROCESS_H
