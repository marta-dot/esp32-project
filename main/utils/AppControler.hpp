#ifndef APP_CONTROLER_H
#define APP_CONTROLER_H

#include "../interfaces/ILed.hpp"
#include "../drivers/Led.hpp"
#include "deepSleepHandler.hpp"
#include "mqttHandler.hpp"
#include <stdio.h>


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "defines.h"
#include <time.h>
#include "cJSON.h"

#include "lwip/err.h"
#include "lwip/sys.h"

class AppControler{
public:
    AppControler() = default;

    void setInterfaces(ILed* led, esp_mqtt_client_handle_t globalMqttClient, DeepSleepHandler* DeepSleepHandler, IMqtt* m_pMqtt );

    void changeBlinkSpeed();

    void run();


private:
    time_t m_lastChanged = {};
    time_t m_endingTime = {};
    bool m_savePending = false;
    int m_newSpeed;

    DeepSleepHandler* m_pDeepSleepHandler = nullptr;
    esp_mqtt_client_handle_t m_pClient = nullptr;
    ILed* m_pLed = nullptr;
    IMqtt* m_pMqtt = nullptr;


    // MqttCLient* pClient;
    
};


#endif
