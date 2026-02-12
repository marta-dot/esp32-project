static const char* LOG_TAG = "NVS";
#define LOG_LOCAL_LEVEL ESP_LOG_INFO

#include "NVS.hpp"


NVS::NVS(){
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }

    handle = nvs::open_nvs_handle("storage", NVS_READWRITE, &err);

}

esp_err_t NVS::getNvsValue(const char* name, int &value){
    esp_err_t err = handle->get_item(name, value);

    switch (err) {
        case ESP_OK:
            printf("Last use value = %d \n", value);
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            printf("The value is not initialized yet!\n");
            value = 10;
            break;
        default :
            printf("Error (%s) reading!\n", esp_err_to_name(err));
    }
    return err;
}

esp_err_t NVS::getNvsValue(const char* name, std::string &text){
    size_t size = 0;
    esp_err_t err = handle->get_item_size(nvs::ItemType::SZ, name, size);

    switch (err) {
        case ESP_OK:
            if (size > 0) {
                char* buf = new char[size];
                err = handle->get_string(name, buf, size);
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
    return err;
}

void NVS::saveNvs(const char* name, int value){
    handle->set_item(name, value);
    handle->commit();
    LOG_INFO("Saving current value = %d \n", value);
}

void NVS::saveNvs(const char* name, const char* text){
    handle->set_string(name, text);
    handle->commit();
    LOG_INFO("Saving current value = %s \n", text);
}
