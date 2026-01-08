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

#define ext_wakeup_pin_0      GPIO_NUM_13
#define WAKEUP_LEVEL          1


void deep_sleep_task(void *args);

void deep_sleep_register_rtc_timer_wakeup(void);

void deep_sleep_register_ext0_wakeup(void);

#endif