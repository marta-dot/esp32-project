#pragma once 
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

class IMqtt {
public:

    static void mqttEventHandler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);
    virtual esp_mqtt_client_handle_t mqttAppStart() = 0;

};