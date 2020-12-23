#include "mqtt_thread_process.h"

Mqtt_thread_process::Mqtt_thread_process()
{

}


void Mqtt_thread_process::run()
{
    Device_public::g_mqtt_process_thread_running = 1;
    demo_mqtt_process_thread(Device_public::g_mqtt_process_thread_running,Device_public::mqtt_handle);

}

Mqtt_thread_process::~Mqtt_thread_process()
{

}
