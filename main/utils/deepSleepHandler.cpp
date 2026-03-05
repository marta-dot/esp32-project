static const char* LOG_TAG = "Deep sleep";
#define LOG_LOCAL_LEVEL ESP_LOG_INFO

#include "deepSleepHandler.hpp"

RTC_DATA_ATTR struct timeval DeepSleepHandler::sleepEnterTime;


void DeepSleepHandler::checkWakeupReason()
{
    struct timeval now;
    gettimeofday(&now, NULL);
    int sleep_time_ms = (now.tv_sec - sleepEnterTime.tv_sec) * 1000 + (now.tv_usec - sleepEnterTime.tv_usec) / 1000;

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
}

esp_err_t DeepSleepHandler::setTimerWakeup(uint32_t seconds) 
{
    esp_err_t err = esp_sleep_enable_timer_wakeup((uint64_t)seconds * 1000000);

    if (err != ESP_OK) {
        return err;
    }

    LOG_INFO("Timer wakeup set to %" PRIu32 " seconds", seconds);
    return err;

}

esp_err_t DeepSleepHandler::setExternalWakeup(gpio_num_t pin, int level) {

    esp_err_t err = esp_sleep_enable_ext0_wakeup(pin, level);

    if (err != ESP_OK) {
        return err;
    }
    
    if (level == 1) {
        rtc_gpio_pullup_dis(pin);
        rtc_gpio_pulldown_en(pin);
    }
    
    LOG_INFO("EXT0 wakeup set on GPIO %d, level %d", pin, level);

    return err;
}

void DeepSleepHandler::start() {
    LOG_WARNING("Entering Deep Sleep now...");

    
    setTimerWakeup(DEEP_SLEEP_TIME_SEC);
    setExternalWakeup((gpio_num_t)BUTTON_GPIO,WAKEUP_LEVEL);
    
    gettimeofday(&sleepEnterTime, NULL);
    vTaskDelay(pdMS_TO_TICKS(100));
    
    esp_deep_sleep_start();
}