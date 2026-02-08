#include <string>

class IStorage{
public:
    virtual ~IStorage() = default;

    virtual void getNvsValue(const std::string& name, int &outValue) = 0;
    virtual void getNvsValue(const std::string& name, std::string &outText) = 0;

    virtual void saveNvs(const std::string& name, int value) = 0;
    virtual void saveNvs(const std::string& name, const std::string& value) = 0;
};