#ifndef MQTT_H
#define MQTT_H

#include "IMqtt.hpp"

#define BROKER_URI "mqtt://test.mosquitto.org:1883" 

class MqttHandler : public IMqtt{

    public:

    MqttHandler() = default;

    static void mqttEventHandler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);
    esp_mqtt_client_handle_t mqttAppStart() override;

};



#endif