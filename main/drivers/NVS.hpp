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
    std::unique_ptr<nvs::NVSHandle> pHandle;       
public:
    NVS();
    ~NVS() = default;

    esp_err_t getNvsValue(const char* pName, int &value) override;
    esp_err_t getNvsValue(const char* pName, std::string &text) override;
    
    void saveNvs(const char* pName, int value) override;
    void saveNvs(const char* pName, const char* pText) override;
     
};
    
#endif