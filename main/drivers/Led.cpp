static const char* LOG_TAG = "led";
#define LOG_LOCAL_LEVEL ESP_LOG_INFO

#include "Led.hpp"


Led::Led(gpio_num_t pin) : pin(pin)
{
    gpio_reset_pin(pin);
    gpio_set_direction(pin, GPIO_MODE_OUTPUT);
    LOG_INFO("Initialized op gpio %d", pin);
}

Led::~Led(){
    stopBlinking();
}

void Led::on(){
    gpio_set_level(pin, 1);
}

void Led::off(){
    gpio_set_level(pin, 0);
}

void Led::blink_task(void* pvParameters){
    BlinkParam* param = static_cast<BlinkParam*>(pvParameters);
    Led* led = param->led;
    int delay = param->delay;

    delete param;

    TickType_t delay_ticks = pdMS_TO_TICKS(delay);
    if (delay_ticks == 0) {
        delay_ticks = 1;
        LOG_WARNING("Blink period %dms is too short. Default to 1 tick.", delay);
    }

    while(1){
        led->on();
        vTaskDelay(delay_ticks);
        led->off();
        vTaskDelay(delay_ticks);
    }
}

void Led::startBlinking(int delay){
    if (blink_task_handle != NULL) {
        LOG_WARNING("Blinking");
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
        LOG_ERROR("Failed to create blink task!");
        blink_task_handle = NULL;
        delete param;
    }
}

void Led::stopBlinking(){
    if (blink_task_handle == NULL) {
        return;
    }

    vTaskDelete(blink_task_handle);

    blink_task_handle = NULL;
    off();
}

