static const char* LOG_TAG = "NVS";
#define LOG_LOCAL_LEVEL ESP_LOG_INFO

#include "nvsHandler.hpp"

esp_err_t setUpNvs(){
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
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

void getNvsValue(std::string name, std::string &text, std::unique_ptr<nvs::NVSHandle> &handle){
    size_t size = 0;
    esp_err_t err = handle->get_item_size(nvs::ItemType::SZ, name.c_str(), size);

    switch (err) {
        case ESP_OK:
            if (size > 0) {
                char* buf = new char[size];
                err = handle->get_string(name.c_str(), buf, size);
                if (err == ESP_OK) {
                    text = std::string(buf);
                    printf("Last use value = %s \n", text.c_str());
                }
                delete[] buf;
            }
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            printf("The value is not initialized yet!\n");
            text = "";
            break;
        default :
            printf("Error (%s) reading!\n", esp_err_to_name(err));
    }
}

void saveNvs(std::string name, std::unique_ptr<nvs::NVSHandle> &handle, int value){
    handle->set_item(name.c_str(), value);
    handle->commit();
    LOG_INFO("Saving current value = %d \n", value);
}

void saveNvs(std::string name, std::unique_ptr<nvs::NVSHandle> &handle, std::string value){
    handle->set_string(name.c_str(), value.c_str());
    handle->commit();
    LOG_INFO("Saving current value = %s \n", value.c_str());
}

