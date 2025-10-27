
class ILed {
    public:
    virtual ~ILed() = default;

    virtual void on() = 0;
    virtual void off() = 0;
    
    virtual void startBlinking(int delay) = 0;
    virtual void stopBlinking() = 0;

};
