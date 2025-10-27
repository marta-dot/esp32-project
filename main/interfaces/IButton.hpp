#include <functional>

class IButton {
    public:
    virtual ~IButton() = default;
    virtual bool isPressed() = 0;
    virtual void setCallback(std::function<void(bool)> callback) = 0;
};
