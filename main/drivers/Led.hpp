#include "../interfaces/ILed.hpp"
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "defines.h"

class Led : public ILed {
private:
    gpio_num_t pin;

    TaskHandle_t blink_task_handle = NULL;

    struct BlinkParam{
        Led* led;
        int delay;
    };

    static void blink_task(void* pvParameters);

public:
    
    explicit Led(gpio_num_t pin);

    ~Led();

    void on() override;
    void off() override;

    void startBlinking(int delay) override;
    void stopBlinking() override;

};