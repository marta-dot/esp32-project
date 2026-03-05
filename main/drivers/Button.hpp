#include "../interfaces/IButton.hpp"
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include <functional>
#include "defines.h"
#include "esp_timer.h"

// typedef void (*FCallback) (bool)>
// typedef std::function<void (bool)> FCallback;

class Button : public IButton {

public:
    Button() = delete;
    explicit Button(gpio_num_t m_pin);

    bool isPressed() override;
    void setCallback(FCallback callback) override;
 
private:
    gpio_num_t m_pin;
    FCallback m_userCallback = nullptr;
    bool m_lastState;
    TaskHandle_t m_buttonTaskHandle = nullptr;

private:
    static void IRAM_ATTR isrHandler(void* pVar);

};
