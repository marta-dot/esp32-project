#include "../interfaces/IButton.hpp"
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include <functional>
#include "defines.h"

class Button : public IButton {
private:
    gpio_num_t pin;
    std::function<void(bool)> user_callback = nullptr;
    volatile bool last_state;
    TaskHandle_t button_task_handle = NULL;
    static const int DEBOUNCE_TIME_MS = 50;

    static void button_task(void* arg);
    static void IRAM_ATTR isr_handler(void* arg);

public:
    explicit Button(gpio_num_t pin);

    bool isPressed() override;
    void setCallback(std::function<void(bool)> callback) override;
 
};
