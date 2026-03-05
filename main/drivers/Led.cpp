static const char* LOG_TAG = "led";
#define LOG_LOCAL_LEVEL ESP_LOG_INFO

#include "Led.hpp"

Led::Led(gpio_num_t m_pin, IStorage* m_storage) : m_pin(m_pin), m_storage(m_storage)
{
    gpio_reset_pin(m_pin);
    gpio_set_direction(m_pin, GPIO_MODE_OUTPUT);
    LOG_INFO("Initialized op gpio %d", m_pin);
    m_blinkSemaphore = xSemaphoreCreateBinary();

    int savedValue = 0;
    if (m_storage->getNvsValue(m_nvsKey, savedValue) == ESP_OK) {
        for(int i=0; i<5; i++) {
            if(m_speeds[i] == savedValue) savedValue = i;
        }
    }
    m_currentDelay = m_speeds[m_currentIndex];
    this->startBlinking(m_currentDelay);
}

Led::~Led(){
    stopBlinking();
}

void Led::on(){
    gpio_set_level(m_pin, 1);
}

void Led::off(){
    gpio_set_level(m_pin, 0);
}

void Led::blinkTask(void* pVar){
    Led* self = static_cast<Led*>(pVar);

    while(1) {
        TickType_t ticks = pdMS_TO_TICKS(self->m_currentDelay);
        if (ticks == 0) ticks = 1;

        self->on();
        xSemaphoreTake(self->m_blinkSemaphore, ticks);
        self->off();
        xSemaphoreTake(self->m_blinkSemaphore, ticks);
    }
}

void Led::startBlinking(int delay){
    m_currentDelay = delay;

    if (m_blinkTaskHandle != nullptr) {
        LOG_WARNING("Blinking");
        return;
    }

    BaseType_t result = xTaskCreate(
        blinkTask,
        "led_blink_task",
        2048,
        this,
        5,
        &m_blinkTaskHandle
    );

    if (result != pdPASS) {
        LOG_ERROR("Failed to create blink task!");
        m_blinkTaskHandle = nullptr;
    }
}

int Led::cycleSpeed() {
    m_currentIndex = (m_currentIndex + 1) % 5;
    m_currentDelay = m_speeds[m_currentIndex];

    xSemaphoreGive(m_blinkSemaphore); // podniesienie
    
    LOG_INFO("Speed changed to: %d ms", m_currentDelay);
    return m_currentDelay;
}

void Led::saveToStorage() {
    m_storage->saveNvs(m_nvsKey, m_speeds[m_currentIndex]);
    LOG_INFO("Saved speed %d to NVS", m_speeds[m_currentIndex]);
}

int Led::getCurrentSpeed() {
    return m_speeds[m_currentIndex];
}

void Led::stopBlinking(){
    if (m_blinkTaskHandle == nullptr) {
        return;
    }

    vTaskDelete(m_blinkTaskHandle);

    m_blinkTaskHandle = nullptr;
    off();
}

