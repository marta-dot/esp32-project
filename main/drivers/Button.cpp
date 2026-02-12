static const char* LOG_TAG = "Button";
#define LOG_LOCAL_LEVEL ESP_LOG_INFO

#include "Button.hpp"

void IRAM_ATTR Button::isr_handler(void* arg) {
    Button* self = static_cast<Button*>(arg);
    
    // Uruchamiamy timer (np. na 50ms). 
    // Jeśli timer już działa, restartujemy go (ignorujać błąd ESP_ERR_INVALID_STATE)
    esp_timer_stop(self->debounce_timer); 
    esp_timer_start_once(self->debounce_timer, 50000); // 50000 us = 50 ms
}

void Button::timer_callback(void* arg) {
    Button* self = static_cast<Button*>(arg);

    if (self->isPressed()) { 
        // Przycisk jest nadal wciśnięty - to nie było zakłócenie
        if (self->user_callback) {
            self->user_callback(true); // Wywołujemy callback tylko dla wciśnięcia
        }
    }
}

Button::Button(gpio_num_t pin) : pin(pin) {
    gpio_config_t io_conf = {};
    io_conf.pin_bit_mask = (1ULL << pin);
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    io_conf.pull_down_en = GPIO_PULLDOWN_ENABLE;
    io_conf.intr_type = GPIO_INTR_POSEDGE;

    gpio_config(&io_conf);
    last_state = isPressed();

    const esp_timer_create_args_t timer_args = {
        .callback = &Button::timer_callback,
        .arg = this,
        .name = "button_debounce"
    };
    esp_timer_create(&timer_args, &debounce_timer);

    gpio_isr_handler_add(pin, isr_handler, this);

}

bool Button::isPressed() {
    return gpio_get_level(pin) == 1;
}

void Button::setCallback(std::function<void(bool)> callback) {
    user_callback = callback;
}

