// Please keep these 2 lines at the beginning of each cpp module - tag and local log level
static const char* LOG_TAG = "Main";
#define LOG_LOCAL_LEVEL ESP_LOG_INFO

#include <stdio.h>
#include "nvs_handle.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

#include "defines.h"
#include "sleep.h"
#include <time.h>
#include "cJSON.h"

#include "drivers/Led.hpp"
#include "drivers/Button.hpp"
#include "drivers/NVS.hpp"
// #include "utils/nvsHandler.hpp"
#include "utils/wifiHandler.hpp"
#include "utils/deepSleepHandler.hpp"
#include "utils/mqttHandler.hpp"
#include <algorithm> 


using namespace std;

#define BLINK_GPIO GPIO_NUM_5
#define BUTTON_GPIO GPIO_NUM_13

// extern esp_mqtt_client_handle_t global_mqtt_client; 
esp_mqtt_client_handle_t global_mqtt_client = nullptr;

void run(void);

void network_task(void* pvParameters) {
    IStorage* storage = static_cast<IStorage*>(pvParameters);
    WifiHandler wifi;

    LOG_INFO("Uruchamianie WiFi...");
    wifi.connect(*storage); 

    LOG_INFO("ruchomienie MQTT...");
    global_mqtt_client = mqtt_app_start();

    vTaskDelete(NULL); 
}

extern "C"
{
    void app_main(void)
    {
        run();
    }
}

void run(void)
{
    esp_log_level_set("*", ESP_LOG_INFO);
    LOG_INFO("Hello from main!");
    TickType_t starting_time = xTaskGetTickCount();
    TickType_t ending_time;

    gpio_install_isr_service(0);

    NVS nvs;

    ILed* led = new Led(BLINK_GPIO, &nvs);
    IButton* button = new Button(BUTTON_GPIO);

    xTaskCreate(network_task, "network_task", 4096, &nvs, 5, NULL);
    
    DeepSleepHandler deepSleep;
    deepSleep.checkWakeupReason();


    volatile time_t last_change_time = 0;
    volatile bool save_pending = false;

    button->setCallback([led, &last_change_time, &save_pending, &starting_time](bool pressed) {
        if (pressed) {
            LOG_INFO("Button is PRESSED");
            int new_speed = led->cycleSpeed();

            if(global_mqtt_client!= nullptr){
                cJSON *json_object = cJSON_CreateObject();
                cJSON_AddStringToObject(json_object, "device", "ESP32_#123");
                cJSON_AddNumberToObject(json_object, "blinking_speed", new_speed);
                char *json_string = cJSON_PrintUnformatted(json_object);
                esp_mqtt_client_publish(global_mqtt_client, "esp/json", json_string, 0, 1, 0);
                free(json_string);
                cJSON_Delete(json_object);
            }else {
                LOG_WARNING("MQTT niepołączone");
            }  

            last_change_time = time(NULL);
            save_pending = true;
            starting_time = xTaskGetTickCount();
            
        } else {
            LOG_INFO("Button is RELEASED");
        }
    });

    
    while (1)
    {
        if(save_pending){
            if(time(NULL) - last_change_time > 5){
                led->saveToStorage();
                save_pending = false;
            }
        }

        ending_time = xTaskGetTickCount();

        if(pdTICKS_TO_MS(ending_time - starting_time) > 60000){
            deepSleep.start();
        }

        SLEEP_MS(1000);

    }


}

