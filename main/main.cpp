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
#include "utils/nvsHandler.hpp"
#include "utils/wifiHandler.hpp"
#include "utils/deepSleepHandler.hpp"
#include "utils/mqttHandler.hpp"
#include <algorithm> 


using namespace std;

#define BLINK_GPIO GPIO_NUM_5
#define BUTTON_GPIO GPIO_NUM_13

void run(void);


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

    //deep sleep
    deep_sleep_register_rtc_timer_wakeup();
    deep_sleep_register_ext0_wakeup();

    esp_err_t err = setUpNvs();
    std::unique_ptr<nvs::NVSHandle> handle = nvs::open_nvs_handle("storage", NVS_READWRITE, &err);

    string ssid;
    string pass;

    if (err == ESP_OK) {
        getNvsValue("ssid",ssid, handle);
        getNvsValue("pass",pass, handle);
        printf("SSID: %s, pass: %s \n", ssid.c_str(), pass.c_str());

        wifi_init_sta(ssid,pass);
    }else{
        wifi_init_sta();
    }

    

    //MQTT
    esp_mqtt_client_handle_t client = mqtt_app_start();

    SLEEP_MS(1000);

    gpio_install_isr_service(0);

    ILed* led = new Led(BLINK_GPIO);
    IButton* button = new Button(BUTTON_GPIO);

    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    }else{

        int blink_delay = 0;
        getNvsValue("blink_delay",blink_delay, handle);

        const int speeds_ms[] = {10, 30, 100, 500, 2000};
        const int num_speeds = sizeof(speeds_ms) / sizeof(speeds_ms[0]);
        auto x = std::find(std::begin(speeds_ms), std::end(speeds_ms), blink_delay);

        int current_speed_index = (x != std::end(speeds_ms))
            ? std::distance(std::begin(speeds_ms), x)
            : -1;
        
        if(current_speed_index == -1){
            current_speed_index = 1;
        }
        
        volatile time_t last_change_time = 0;
        volatile int speed_to_save = 0;
        volatile bool save_pending = false;

        led->startBlinking(speeds_ms[current_speed_index]);

        button->setCallback([led, &current_speed_index, speeds_ms, num_speeds, &last_change_time, &speed_to_save, &save_pending, client](bool pressed) {
            if (pressed) {
                LOG_INFO("Button is PRESSED");
                led->stopBlinking();
                current_speed_index = (current_speed_index + 1) % num_speeds;
                int new_speed = speeds_ms[current_speed_index];
                led->startBlinking(new_speed);

                cJSON *json_object = cJSON_CreateObject();
                cJSON_AddStringToObject(json_object, "device", "ESP32_#123");
                cJSON_AddNumberToObject(json_object, "blinking_speed", new_speed);
                char *json_string = cJSON_PrintUnformatted(json_object);
                esp_mqtt_client_publish(client, "esp/json", json_string, 0, 1, 0);
                free(json_string);
                cJSON_Delete(json_object);

                last_change_time = time(NULL);
                speed_to_save = new_speed;
                save_pending = true;
                
            } else {
                LOG_INFO("Button is RELEASED");
            }
        });


        while (1)
        {
            if(save_pending){
                if(time(NULL) - last_change_time > 5){
                    saveNvs("blink_delay", handle, speed_to_save);
                    save_pending = false;
                }
            }

            ending_time = xTaskGetTickCount();

            if(pdTICKS_TO_MS(ending_time - starting_time) > 60000){
                xTaskCreate(deep_sleep_task, "deep_sleep_task", 4096, NULL, 6, NULL);
            }

            SLEEP_MS(1000);

        }
    }

}

