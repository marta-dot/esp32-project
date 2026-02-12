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
    gpio_num_t pin;
    IStorage* storage;
    const int speeds[5] = {10, 30, 100, 500, 2000};
    int currentIndex = 1;
    const char* nvsKey = "blink_delay";
    volatile int current_delay = 10;
    TaskHandle_t blink_task_handle = nullptr;
    SemaphoreHandle_t blink_semaphore = nullptr;

    static void blink_task(void* pvParameters);

public:

    Led() = delete;
    explicit Led(gpio_num_t pin, IStorage* storage);

    ~Led();

    void on() override;
    void off() override;

    void startBlinking(int delay) override;
    void stopBlinking() override;

    int cycleSpeed() override; // Przełącza na następną prędkość
    void saveToStorage() override;
    int getCurrentSpeed() override;

};