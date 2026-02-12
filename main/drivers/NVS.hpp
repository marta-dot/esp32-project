#ifndef NVS_H
#define NVS_H

#include "../interfaces/IStorage.hpp"
#include <inttypes.h>
#include "nvs_flash.h"
#include "nvs.h"
#include "nvs_handle.hpp"
#include "esp_log.h"
#include "defines.h" 

class NVS : public IStorage {
private:
    std::unique_ptr<nvs::NVSHandle> handle;       
public:
    NVS();
    ~NVS() = default;

    esp_err_t getNvsValue(const char* name, int &value) override;
    esp_err_t getNvsValue(const char* name, std::string &text) override;
    
    void saveNvs(const char* name, int value) override;
    void saveNvs(const char* name, const char* text) override;
     
};
    
#endif