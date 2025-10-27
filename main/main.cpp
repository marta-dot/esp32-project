// Please keep these 2 lines at the beginning of each cpp module - tag and local log level
static const char* LOG_TAG = "Main";
#define LOG_LOCAL_LEVEL ESP_LOG_INFO
// #define LOG_LOCAL_LEVEL ESP_LOG_DEBUG

#include "defines.h"
#include "sleep.h"
// #include "driver/gpio.h"

#include "drivers/Led.hpp"
#include "drivers/Button.hpp"

#define BLINK_GPIO GPIO_NUM_5
#define BUTTON_GPIO GPIO_NUM_16

// static uint8_t s_led_state = 0;

void run(void);

extern "C"
{
    void app_main(void)
    {
        run();
    }
}

void run(void)
{
    LOG_INFO("Hello from main!");
    gpio_install_isr_service(0);

    ILed* led = new Led(BLINK_GPIO, true);
    IButton* button = new Button(BUTTON_GPIO);

    const int speeds_ms[] = {10, 30, 100, 500, 2000};

    const int num_speeds = sizeof(speeds_ms) / sizeof(speeds_ms[0]);
    int current_speed_index = 0;
    led->startBlinking(current_speed_index);

    button->setCallback([led, &current_speed_index, speeds_ms, num_speeds](bool pressed) {
        if (pressed) {
            LOG_INFO("Button is PRESSED");
            led->stopBlinking();
            current_speed_index = (current_speed_index + 1) % num_speeds;
            int new_speed = speeds_ms[current_speed_index];
            led->startBlinking(new_speed);
        } else {
            LOG_INFO("Button is RELEASED");
        }
    });

    while (1)
    {
        LOG_DEBUG("Hello loop");
        // LOG_WARNING("Warning");
        // LOG_DEBUG("Hello loop2");
        // LOG_ERROR("Error");

        // led->on();
        // SLEEP_MS(1000);
    
        // led->off();
        // SLEEP_MS(1000);  

        //buton
       
        // if(button->isPressed()) {
        //     printf("Przycisk nacisniety!\n");
        // }
        
        SLEEP_MS(1000);
        

    }


}

