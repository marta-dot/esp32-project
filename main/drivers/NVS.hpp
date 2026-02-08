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

    void getNvsValue(const std::string& name, int &blink_delay) override;
    void getNvsValue(const std::string& name, std::string &outText) override;
    
    void saveNvs(const std::string& name, int value) override;
    void saveNvs(const std::string& name, const std::string& value) override;
     
};
    
#endif