static const char* LOG_TAG = "Deep sleep";
#define LOG_LOCAL_LEVEL ESP_LOG_INFO

#include "deepSleepHandler.hpp"

static struct timeval sleep_enter_time;

void deep_sleep_task(void *args)
{

    struct timeval now;
    gettimeofday(&now, NULL);
    int sleep_time_ms = (now.tv_sec - sleep_enter_time.tv_sec) * 1000 + (now.tv_usec - sleep_enter_time.tv_usec) / 1000;

    uint32_t causes = esp_sleep_get_wakeup_cause();
    if (causes & BIT(ESP_SLEEP_WAKEUP_UNDEFINED)) {
        printf("Not a deep sleep reset\n");
    } else {
        if (causes & BIT(ESP_SLEEP_WAKEUP_TIMER)) {
            printf("Wake up from timer. Time spent in deep sleep: %dms\n", sleep_time_ms);
        }
        if (causes & BIT(ESP_SLEEP_WAKEUP_EXT0)) {
            printf("Wake up from ext0\n");
        }
    }

    vTaskDelay(1000 / portTICK_PERIOD_MS);

    LOG_INFO("Entering deep sleep");

    gettimeofday(&sleep_enter_time, NULL);
    
    esp_deep_sleep_start();
}

void deep_sleep_register_rtc_timer_wakeup(void)
{
    const int wakeup_time_sec = 60;
    printf("Enabling timer wakeup, %ds\n", wakeup_time_sec);
    ESP_ERROR_CHECK(esp_sleep_enable_timer_wakeup(wakeup_time_sec * 1000000));
}

void deep_sleep_register_ext0_wakeup(void)
{
    printf("Enabling EXT0 wakeup on pin GPIO%d\n", ext_wakeup_pin_0);
    ESP_ERROR_CHECK(esp_sleep_enable_ext0_wakeup(ext_wakeup_pin_0, WAKEUP_LEVEL));
    ESP_ERROR_CHECK(rtc_gpio_pullup_dis(ext_wakeup_pin_0));
    ESP_ERROR_CHECK(rtc_gpio_pulldown_en(ext_wakeup_pin_0));
}