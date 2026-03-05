#include "AppControler.hpp"
#include "sleep.h"

void AppControler::setInterfaces(ILed* led, esp_mqtt_client_handle_t globalMqttClient, DeepSleepHandler* DeepSleepHandler, IMqtt* pMqtt){
    m_pLed = led;
    m_pClient = globalMqttClient;
    m_pDeepSleepHandler = DeepSleepHandler;
    m_pMqtt = pMqtt;
    
};

void AppControler::changeBlinkSpeed()
{
    m_newSpeed = m_pLed->cycleSpeed();

    m_lastChanged = time(NULL);
    m_savePending = true;

    if(!m_pClient)
    {
        return;
    }
    cJSON *jsonObject = cJSON_CreateObject();
    cJSON_AddStringToObject(jsonObject, "device", "ESP32_#123");
    cJSON_AddNumberToObject(jsonObject, "blinking_speed", m_newSpeed);
    char *jsonString = cJSON_PrintUnformatted(jsonObject);
    esp_mqtt_client_publish(m_pClient, "esp/json", jsonString, 0, 1, 0);
    free(jsonString);
    cJSON_Delete(jsonObject);
    
    

};

void AppControler::run()
{
    while (true)
    {
        if(m_savePending){
            if(time(NULL) - m_lastChanged > 5){
                m_pLed->saveToStorage();
                m_savePending = false;
            }
        }

        m_endingTime = time(NULL);

        if(pdTICKS_TO_MS(m_endingTime - m_lastChanged) > 60000){
            m_pDeepSleepHandler->start();
        }

        SLEEP_MS(1000);

    }
}
