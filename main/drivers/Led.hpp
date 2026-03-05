#pragma once 
#include "../interfaces/ILed.hpp"
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "defines.h"
#include "../interfaces/IStorage.hpp"
#include "freertos/semphr.h"


class Led : public ILed {
private:
    gpio_num_t m_pin;
    IStorage* m_storage;
    const int m_speeds[5] = {10, 30, 100, 500, 2000};
    int m_currentIndex = 1;
    const char* m_nvsKey = "blink_delay";
    volatile int m_currentDelay = 10;
    TaskHandle_t m_blinkTaskHandle = nullptr;
    SemaphoreHandle_t m_blinkSemaphore = nullptr;

    static void blinkTask(void* pVar);

public:

    Led() = delete;
    explicit Led(gpio_num_t m_pin, IStorage* m_storage);

    ~Led();

    void on() override;
    void off() override;

    void startBlinking(int delay) override;
    void stopBlinking() override;

    int cycleSpeed() override; // Przełącza na następną prędkość
    void saveToStorage() override;
    int getCurrentSpeed() override;

};