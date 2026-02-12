static const char* LOG_TAG = "led";
#define LOG_LOCAL_LEVEL ESP_LOG_INFO

#include "Led.hpp"

int x[] = {10,20,50,200,1000};

Led::Led(gpio_num_t pin, IStorage* storage) : pin(pin), storage(storage)
{
    gpio_reset_pin(pin);
    gpio_set_direction(pin, GPIO_MODE_OUTPUT);
    LOG_INFO("Initialized op gpio %d", pin);
    blink_semaphore = xSemaphoreCreateBinary();

    int savedValue;
    if (storage->getNvsValue(nvsKey, savedValue) == ESP_OK) {
        for(int i=0; i<5; i++) {
            if(speeds[i] == savedValue) currentIndex = i;
        }
    }
    current_delay = speeds[currentIndex];
    this->startBlinking(current_delay);
}

Led::~Led(){
    stopBlinking();
}

void Led::on(){
    gpio_set_level(pin, 1);
}

void Led::off(){
    gpio_set_level(pin, 0);
}

void Led::blink_task(void* pvParameters){
    Led* self = static_cast<Led*>(pvParameters);

    while(1) {
        TickType_t ticks = pdMS_TO_TICKS(self->current_delay);
        if (ticks == 0) ticks = 1;

        self->on();
        xSemaphoreTake(self->blink_semaphore, ticks);
        self->off();
        xSemaphoreTake(self->blink_semaphore, ticks);
    }
}

void Led::startBlinking(int delay){
    current_delay = delay;

    if (blink_task_handle != nullptr) {
        LOG_WARNING("Blinking");
        return;
    }

    BaseType_t result = xTaskCreate(
        blink_task,
        "led_blink_task",
        2048,
        this,
        5,
        &blink_task_handle
    );

    if (result != pdPASS) {
        LOG_ERROR("Failed to create blink task!");
        blink_task_handle = nullptr;
    }
}

int Led::cycleSpeed() {
    currentIndex = (currentIndex + 1) % 5;
    current_delay = speeds[currentIndex];

    xSemaphoreGive(blink_semaphore); // podniesienie
    
    LOG_INFO("Speed changed to: %d ms", current_delay);
    return current_delay;
}

void Led::saveToStorage() {
    storage->saveNvs(nvsKey, speeds[currentIndex]);
    LOG_INFO("Saved speed %d to NVS", speeds[currentIndex]);
}

int Led::getCurrentSpeed() {
    return speeds[currentIndex];
}

void Led::stopBlinking(){
    if (blink_task_handle == nullptr) {
        return;
    }

    vTaskDelete(blink_task_handle);

    blink_task_handle = nullptr;
    off();
}

