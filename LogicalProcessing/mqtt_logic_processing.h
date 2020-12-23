#ifndef MQTT_LOGIC_PROCESSING_H
#define MQTT_LOGIC_PROCESSING_H

#include <QObject>
#include <QTime>

class MqttLogicProcessing : public QObject
{
    Q_OBJECT
public:
    explicit MqttLogicProcessing(QObject *parent = 0);
    void mqtt_logic_get_mode();
    void mqtt_logic_post_version();
    void mqtt_logic_work_data();
signals:

    
public slots:
    
};

#endif // MQTT_LOGIC_PROCESSING_H
