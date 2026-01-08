#ifndef NVS_H
#define NVS_H

#include <inttypes.h>
#include "nvs_flash.h"
#include "nvs.h"
#include "nvs_handle.hpp"
#include "esp_log.h"
#include "defines.h" 

esp_err_t setUpNvs();

void getNvsValue(std::string name, int &blink_delay, std::unique_ptr<nvs::NVSHandle> &handle);

void getNvsValue(std::string name, std::string &text, std::unique_ptr<nvs::NVSHandle> &handle);

void saveNvs(std::string name, std::unique_ptr<nvs::NVSHandle> &handle, int value);

void saveNvs(std::string name, std::unique_ptr<nvs::NVSHandle> &handle, std::string value);

#endif