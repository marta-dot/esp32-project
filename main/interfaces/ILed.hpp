
class ILed {
    public:
    virtual ~ILed() = default;

    virtual void on() = 0;
    virtual void off() = 0;
    
    virtual void startBlinking(int delay) = 0;
    virtual void stopBlinking() = 0;

    virtual int cycleSpeed() = 0;         // Przełącza na następną prędkość
    virtual void saveToStorage() = 0;
    virtual int getCurrentSpeed() = 0;

};
