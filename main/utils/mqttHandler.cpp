static const char* LOG_TAG = "MQTT";
#define LOG_LOCAL_LEVEL ESP_LOG_INFO

#include "mqttHandler.hpp"


void MqttHandler::mqttEventHandler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{

    esp_mqtt_event_handle_t event = (esp_mqtt_event_handle_t)event_data;
    esp_mqtt_client_handle_t client = event->client;

    switch ((esp_mqtt_event_id_t)event_id) {
    case MQTT_EVENT_CONNECTED:
        LOG_INFO( "MQTT_EVENT_CONNECTED");

        esp_mqtt_client_subscribe(client, "/esp/json", 0);
        LOG_INFO( "sent subscribe successful");

        esp_mqtt_client_publish(client, "/esp/json", "Hi :)", 0, 1, 0);
        LOG_INFO( "sent publish successful");
        break;
    
    case MQTT_EVENT_DISCONNECTED:
        LOG_INFO( "MQTT_EVENT_DISCONNECTED");
        break;

    case MQTT_EVENT_SUBSCRIBED:
        LOG_INFO( "MQTT_EVENT_SUBSCRIBED");
        break;
    
    case MQTT_EVENT_UNSUBSCRIBED:
        LOG_INFO( "MQTT_EVENT_UNSUBSCRIBED");
        break;

    case MQTT_EVENT_PUBLISHED:
        LOG_INFO( "MQTT_EVENT_PUBLISHED");
        break;

    case MQTT_EVENT_DATA:
        LOG_INFO("MQTT_EVENT_DATA");
        printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
        printf("DATA=%.*s\r\n", event->data_len, event->data);
        break;

    case MQTT_EVENT_ERROR:
        LOG_INFO("MQTT_EVENT_ERROR");
        break;
        
    default:
        break;
    }
}

esp_mqtt_client_handle_t MqttHandler::mqttAppStart()
{
    esp_mqtt_client_config_t mqtt_cfg = {};
    mqtt_cfg.broker.address.uri = BROKER_URI;

    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, (esp_mqtt_event_id_t)ESP_EVENT_ANY_ID, mqttEventHandler, NULL);
    esp_mqtt_client_start(client);

    return client;
}
