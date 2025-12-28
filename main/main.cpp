// Please keep these 2 lines at the beginning of each cpp module - tag and local log level
static const char* LOG_TAG = "Main";
#define LOG_LOCAL_LEVEL ESP_LOG_INFO
// #define LOG_LOCAL_LEVEL ESP_LOG_DEBUG

#include <stdio.h>
#include "nvs_handle.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

#include "defines.h"
#include "sleep.h"
#include <time.h>

#include "drivers/Led.hpp"
#include "drivers/Button.hpp"
#include <algorithm> 

using namespace std;

#define BLINK_GPIO GPIO_NUM_5
#define BUTTON_GPIO GPIO_NUM_16

// static uint8_t s_led_state = 0;

void run(void);

extern "C"
{
    void app_main(void)
    {
        run();
    }
}

int32_t readNVS(nvs_handle_t my_handle, esp_err_t err){
    int32_t value = 0;
    LOG_INFO("\nReading counter from NVS...");
    err = nvs_get_i32(my_handle, "last_delay", &value);
    switch (err) {
        case ESP_OK:
        LOG_INFO("Read counter = %" PRIu32, value);
        return value;
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            LOG_WARNING("The value is not initialized yet!");
            break;
        default:
            LOG_ERROR("Error (%s) reading!", esp_err_to_name(err));
    }
    return -1;
}


void writeNVS(nvs_handle_t my_handle, esp_err_t err, int32_t value){
    err = nvs_set_i32(my_handle, "last_delay", value);
    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        LOG_ERROR("Failed to commit NVS changes!");
    }else
    {
        LOG_INFO("ESP_OK");
    }
    
}

void run(void)
{
    LOG_INFO("Hello from main!");
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );

    std::unique_ptr<nvs::NVSHandle> handle = nvs::open_nvs_handle("storage", NVS_READWRITE, &err);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        printf("Done\n");
        // Read
        printf("Reading restart counter from NVS ... ");
        int32_t restart_counter = 0; // value will default to 0, if not set yet in NVS
        err = handle->get_item("restart_counter", restart_counter);
        switch (err) {
            case ESP_OK:
                printf("Done\n");
                printf("Restart counter = %" PRIu32 "\n", restart_counter);
                break;
            case ESP_ERR_NVS_NOT_FOUND:
                printf("The value is not initialized yet!\n");
                break;
            default :
                printf("Error (%s) reading!\n", esp_err_to_name(err));
        }

        // Write
        printf("Updating restart counter in NVS ... ");
        restart_counter++;
        err = handle->set_item("restart_counter", restart_counter);
        printf((err != ESP_OK) ? "Failed!\n" : "Done\n");

        // Commit written value.
        // After setting any values, nvs_commit() must be called to ensure changes are written
        // to flash storage. Implementations may write to storage at other times,
        // but this is not guaranteed.
        printf("Committing updates in NVS ... ");
        err = handle->commit();
        printf((err != ESP_OK) ? "Failed!\n" : "Done\n");
    }

    printf("\n");

    // gpio_install_isr_service(0);

    // ILed* led = new Led(BLINK_GPIO);
    // IButton* button = new Button(BUTTON_GPIO);

    // const int speeds_ms[] = {10, 30, 100, 500, 2000};
    // const int num_speeds = sizeof(speeds_ms) / sizeof(speeds_ms[0]);
    // auto x = std::find(std::begin(speeds_ms), std::end(speeds_ms), blink_delay);

    // int current_speed_index = (x != std::end(speeds_ms))
    //     ? std::distance(std::begin(speeds_ms), x)
    //     : -1;
    
    // if(current_speed_index == -1){
    //     current_speed_index = 1;
    // }
    
    // volatile time_t last_change_time = 0;
    // volatile int speed_to_save = 0;
    // volatile bool save_pending = false;

    // led->startBlinking(speeds_ms[current_speed_index]);

    // button->setCallback([led, &current_speed_index, speeds_ms, num_speeds, &last_change_time, &speed_to_save, &save_pending](bool pressed) {
    //     if (pressed) {
    //         LOG_INFO("Button is PRESSED");
    //         led->stopBlinking();
    //         current_speed_index = (current_speed_index + 1) % num_speeds;
    //         int new_speed = speeds_ms[current_speed_index];
    //         led->startBlinking(new_speed);

    //         last_change_time = time(NULL);
    //         speed_to_save = new_speed;
    //         save_pending = true;
            
    //     } else {
    //         LOG_INFO("Button is RELEASED");
    //     }
    // });

    // while (1)
    // {
    //     LOG_DEBUG("Hello loop");

    //     if(save_pending){
    //         if(time(NULL) - last_change_time > 5){
    //             writeNVS(my_handle, err, speed_to_save);
    //             save_pending = false;
    //         }
    //     }


    //     // LOG_WARNING("Warning");
    //     // LOG_DEBUG("Hello loop2");
    //     // LOG_ERROR("Error");

    //     // led->on();
    //     // SLEEP_MS(1000);
    
    //     // led->off();
    //     // SLEEP_MS(1000);  
       
    //     // if(button->isPressed()) {
    //     //     printf("Przycisk nacisniety!\n");
    //     // }
        
    //     SLEEP_MS(1000);
        

    // }

    for (int i = 10; i >= 0; i--) {
        printf("Restarting in %d seconds...\n", i);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    printf("Restarting now.\n");
    fflush(stdout);
    esp_restart();


}

