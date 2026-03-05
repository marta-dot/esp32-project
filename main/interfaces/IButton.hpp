#include <functional>

typedef std::function<void ()> FCallback;

class IButton {
    public:
    virtual ~IButton() = default;
    virtual bool isPressed() = 0;
    virtual void setCallback(FCallback callback) = 0;
};
