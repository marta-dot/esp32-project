#ifndef NVS_H
#define NVS_H

#include <inttypes.h>
#include "nvs_flash.h"
#include "nvs.h"
#include "nvs_handle.hpp"

esp_err_t setUpNvs(){
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );
    return err;
}

void getNvsValue(std::string name, int &blink_delay, std::unique_ptr<nvs::NVSHandle> &handle){
    esp_err_t err = handle->get_item(name.c_str(), blink_delay);

    switch (err) {
        case ESP_OK:
            printf("Last use value = %d \n", blink_delay);
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            printf("The value is not initialized yet!\n");
            blink_delay = 10;
            break;
        default :
            printf("Error (%s) reading!\n", esp_err_to_name(err));
    }
}

void saveNvs(std::string name, std::unique_ptr<nvs::NVSHandle> &handle, int value){
    esp_err_t err = handle->set_item(name.c_str(), value);
    err = handle->commit();
    LOG_INFO("Saving current value = %d \n", value);
}

#endif