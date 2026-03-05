static const char* LOG_TAG = "NVS";
#define LOG_LOCAL_LEVEL ESP_LOG_INFO

#include "NVS.hpp"


NVS::NVS(){
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }

    pHandle = nvs::open_nvs_handle("storage", NVS_READWRITE, &err);

}

esp_err_t NVS::getNvsValue(const char* pName, int &value){
    esp_err_t err = pHandle->get_item(pName, value);

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

esp_err_t NVS::getNvsValue(const char* pName, std::string &text){
    size_t size = 0;
    esp_err_t err = pHandle->get_item_size(nvs::ItemType::SZ, pName, size);

    switch (err) {
        case ESP_OK:
            if (size > 0) {
                char* buf = new char[size];
                err = pHandle->get_string(pName, buf, size);
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

void NVS::saveNvs(const char* pName, int value){
    pHandle->set_item(pName, value);
    pHandle->commit();
    LOG_INFO("Saving current value = %d \n", value);
}

void NVS::saveNvs(const char* pName, const char* pText){
    pHandle->set_string(pName, pText);
    pHandle->commit();
    LOG_INFO("Saving current value = %s \n", pText);
}
