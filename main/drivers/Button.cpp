static const char* LOG_TAG = "Button";
#define LOG_LOCAL_LEVEL ESP_LOG_INFO

#include "Button.hpp"

void IRAM_ATTR Button::isrHandler(void* pVar) {
    Button* pInstance = static_cast<Button*>(pVar);
    pInstance->m_userCallback(true);
}

Button::Button(gpio_num_t m_pin) : 
m_pin(m_pin) 
{
    gpio_config_t io_conf = {};
    io_conf.pin_bit_mask = (1ULL << m_pin);
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    io_conf.pull_down_en = GPIO_PULLDOWN_ENABLE;
    io_conf.intr_type = GPIO_INTR_POSEDGE;

    gpio_config(&io_conf);
    m_lastState = isPressed();

    gpio_isr_handler_add(m_pin, isrHandler, this);

}

bool Button::isPressed() {
    return gpio_get_level(m_pin) == 1;
}

void Button::setCallback(FCallback callback) {
    m_userCallback = callback;
}

