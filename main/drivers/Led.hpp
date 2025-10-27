#include "../interfaces/ILed.hpp"
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

class Led : public ILed {
private:
    gpio_num_t pin;
    bool active_led;
    static constexpr const char* TAG = "Led";

    TaskHandle_t blink_task_handle = NULL;

    struct BlinkParam{
        Led* led;
        int delay;
    };

    static void blink_task(void* pvParameters){
        BlinkParam* param = static_cast<BlinkParam*>(pvParameters);
        Led* led = param->led;
        int delay = param->delay;

        delete param;

        TickType_t delay_ticks = pdMS_TO_TICKS(delay);
        if (delay_ticks == 0) {
            delay_ticks = 1;
            ESP_LOGW(TAG, "Blink period %dms is too short. Defaulting to 1 tick.", delay);
        }

        while(1){
            led->on();
            vTaskDelay(delay_ticks);
            led->off();
            vTaskDelay(delay_ticks);
        }

    }

public:
    
    explicit Led(gpio_num_t pin, bool active_led = true) : pin(pin), active_led(active_led)
    {
        gpio_reset_pin(pin);
        gpio_set_direction(pin, GPIO_MODE_OUTPUT);
        ESP_LOGI(TAG, "Led initialized op gpio %d", pin);
    }

    ~Led(){
        stopBlinking();
    }

    void on() override{
        gpio_set_level(pin, 1);
    }

    void off() override{
        gpio_set_level(pin, 0);
    }

    void startBlinking(int delay) override{
        if (blink_task_handle != NULL) {
            ESP_LOGW(TAG, "Blink task already running!");
            return;
        }

        BlinkParam* param = new BlinkParam();
        param->led = this;
        param->delay = delay;

        BaseType_t result = xTaskCreate(
            blink_task,
            "led_blink_task",
            2048,
            (void*)param,
            5,
            &blink_task_handle
        );

        if (result != pdPASS) {
            ESP_LOGE(TAG, "Failed to create blink task!");
            blink_task_handle = NULL;
            delete param;
        }
    }

    void stopBlinking() override{
        if (blink_task_handle == NULL) {
            return;
        }

        vTaskDelete(blink_task_handle);

        blink_task_handle = NULL;
        off();
    }

};