#ifndef IStorage_H
#define IStorage_H

#include <string>
#include "esp_err.h"

class IStorage{
public:
    virtual ~IStorage() = default;

    virtual esp_err_t getNvsValue(const char* name, int &value) = 0;
    virtual esp_err_t getNvsValue(const char* name, std::string &text) = 0;

    virtual void saveNvs(const char* name, int value) = 0;
    virtual void saveNvs(const char* name, const char* text) = 0;
};

#endif