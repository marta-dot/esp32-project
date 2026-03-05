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
#include "wifiHandler.hpp"
#include "utils/deepSleepHandler.hpp"
#include "utils/mqttHandler.hpp"
#include "utils/AppControler.hpp"

#include <algorithm> 


// using namespace std;

#define BLINK_GPIO GPIO_NUM_5
#define BUTTON_GPIO GPIO_NUM_13

esp_mqtt_client_handle_t globalMqttClient = nullptr;

void run(void);

void networkTask(void* pvParameters) 
{
    IStorage* storage = static_cast<IStorage*>(pvParameters);
    WifiHandler wifi;

    LOG_INFO("Uruchamianie WiFi...");
    wifi.connect(*storage); 

    LOG_INFO("ruchomienie MQTT...");
    // auto mqtt = MqttHandler();
    // globalMqttClient = mqtt.mqttAppStart();

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

    gpio_install_isr_service(0);

    NVS nvs;
    auto led = Led(BLINK_GPIO, &nvs);
    IButton* button = new Button(BUTTON_GPIO);

    xTaskCreate(networkTask, "network_task", 4096, &nvs, 5, NULL);

    auto mqtt = MqttHandler();
    
    
    DeepSleepHandler deepSleep;
    deepSleep.checkWakeupReason();
    AppControler appControler;

    appControler.setInterfaces(&led, globalMqttClient, &deepSleep, &mqtt);

    auto callback = [&appControler]()
    {
        appControler.changeBlinkSpeed();
    };
    button->setCallback(callback);

    appControler.run();


}

