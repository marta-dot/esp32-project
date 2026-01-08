static const char* LOG_TAG = "Button";
#define LOG_LOCAL_LEVEL ESP_LOG_INFO

#include "Button.hpp"


void Button::button_task(void* arg){
    Button* button = static_cast<Button*>(arg);
    LOG_INFO("Task started for GPIO %d", button->pin);

    while(1){
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        vTaskDelay(pdMS_TO_TICKS(DEBOUNCE_TIME_MS));

        bool new_state = button->isPressed();

        if (new_state != button->last_state) {
            button->last_state = new_state;

            if (button->user_callback) {
                button->user_callback(new_state);
            }
        }
    }
}

void IRAM_ATTR Button::isr_handler(void* arg) {
    Button* button = static_cast<Button*>(arg);
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    vTaskNotifyGiveFromISR(button->button_task_handle, &xHigherPriorityTaskWoken);

    if (xHigherPriorityTaskWoken) {
        portYIELD_FROM_ISR();
    }
}


Button::Button(gpio_num_t pin) : pin(pin) {
    gpio_config_t io_conf = {};
    io_conf.pin_bit_mask = (1ULL << pin);
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    io_conf.pull_down_en = GPIO_PULLDOWN_ENABLE;
    io_conf.intr_type = GPIO_INTR_ANYEDGE;

    gpio_config(&io_conf);
    last_state = isPressed();

    xTaskCreate(
        button_task,
        "button_task",
        2048,
        (void*)this,
        10,
        &button_task_handle
    );

    gpio_isr_handler_add(pin, isr_handler, (void*)this);
    LOG_INFO("Initialized on GPIO %d (Task: %p)", pin, button_task_handle);
}

bool Button::isPressed() {
    return gpio_get_level(pin) == 1;
}

void Button::setCallback(std::function<void(bool)> callback) {
    user_callback = callback;
}

