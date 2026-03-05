#ifndef DEEP_SLEEP_H
#define DEEP_SLEEP_H

#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include "sdkconfig.h"
#include "soc/soc_caps.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_sleep.h"
#include "esp_log.h"
#include "driver/rtc_io.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "driver/gpio.h"
#include "defines.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <inttypes.h>
#include "esp_check.h"

#define BUTTON_GPIO GPIO_NUM_13
#define DEEP_SLEEP_TIME_SEC 60
#define WAKEUP_LEVEL 1

class DeepSleepHandler {
private:
    static RTC_DATA_ATTR struct timeval sleepEnterTime;

public:

    esp_err_t setTimerWakeup(uint32_t seconds);
    esp_err_t setExternalWakeup(gpio_num_t pin, int level);

    void checkWakeupReason();
    void start();
};

#endif