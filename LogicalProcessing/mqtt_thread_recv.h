#ifndef MQTT_THREAD_RECV_H
#define MQTT_THREAD_RECV_H

#include <QObject>
#include <QThread>
#include <QMutex>

#include "public/device_public.h"
#include "external/ali_ca_cert.h"
#include "portfiles/aiot_port/posix_port.h"
#include "LinkSDK/core/aiot_mqtt_api.h"
#include "LinkSDK/ntp_posix.h"

class Mqtt_thread_recv : public QThread
{
public:
    Mqtt_thread_recv();
    ~Mqtt_thread_recv();

private:
    QMutex mutex;

protected:
    void run();
};

#endif // MQTT_THREAD_RECV_H
