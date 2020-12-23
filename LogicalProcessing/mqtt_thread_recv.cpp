#include "mqtt_thread_recv.h"

Mqtt_thread_recv::Mqtt_thread_recv()
{

}

void Mqtt_thread_recv::run()
{
    Device_public::g_mqtt_recv_thread_running = 1;
    demo_mqtt_recv_thread(Device_public::g_mqtt_recv_thread_running,Device_public::mqtt_handle);

}

Mqtt_thread_recv::~Mqtt_thread_recv()
{


}
