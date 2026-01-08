#ifndef MQTT_H
#define MQTT_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "defines.h"
#include "mqtt_client.h"
#include "esp_log.h"

#define BROKER_URI "mqtt://test.mosquitto.org:1883" 

void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);

esp_mqtt_client_handle_t mqtt_app_start(void);


#endif