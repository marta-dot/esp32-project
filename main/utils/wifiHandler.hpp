#ifndef WIFI_H
#define WIFI_H

#include <string>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_timer.h"
#include <stdio.h>
#include "esp_check.h"
#include "defines.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "../interfaces/IStorage.hpp"

#define EXAMPLE_ESP_WIFI_SSID      "ssid"
#define EXAMPLE_ESP_WIFI_PASS      "pass"

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1


class WifiHandler {
    EventGroupHandle_t s_wifi_event_group;
    TickType_t start_time;
    TickType_t end_time;
    std::string ssid;
    std::string pass;
public:
    static void event_handler(void* arg, esp_event_base_t event_base,
                                   int32_t event_id, void* event_data);
    esp_err_t connect(std::string ssid, std::string pass);
    void connect(IStorage& storage);
    esp_err_t waitForConnection();
    void handleConnectionFailure(esp_err_t err);
};
#endif