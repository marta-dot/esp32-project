#ifndef IStorage_H
#define IStorage_H

#include <string>
#include "esp_err.h"

class IStorage{
public:
    virtual ~IStorage() = default;

    virtual esp_err_t getNvsValue(const char* pName, int &value) = 0;
    virtual esp_err_t getNvsValue(const char* pName, std::string &text) = 0;

    virtual void saveNvs(const char* pName, int value) = 0;
    virtual void saveNvs(const char* pName, const char* pText) = 0;
};

#endif