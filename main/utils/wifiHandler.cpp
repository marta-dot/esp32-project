static const char* LOG_TAG = "wifi";
#define LOG_LOCAL_LEVEL ESP_LOG_INFO

#include "wifiHandler.hpp"

void WifiHandler::eventHandler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
    WifiHandler* self = static_cast<WifiHandler*>(arg);

    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        esp_wifi_connect();
        LOG_INFO("connect to the AP fail"); 
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        LOG_INFO("got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        xEventGroupSetBits(self->m_wifiEventGroup, WIFI_CONNECTED_BIT);
    }
}

esp_err_t WifiHandler::connect(std::string ssid, std::string pass)
{
    
    m_wifiEventGroup = xEventGroupCreate();

    ESP_RETURN_ON_ERROR(esp_netif_init(), LOG_TAG, "esp_netif_init");

    ESP_RETURN_ON_ERROR(esp_event_loop_create_default(), LOG_TAG, "esp_event_loop_create_default");

    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();

    ESP_RETURN_ON_ERROR(esp_wifi_init(&cfg), LOG_TAG, "Wifi init failed");

    esp_event_handler_instance_t instance_any_id = {};
    esp_event_handler_instance_t instance_got_ip = {};
    ESP_RETURN_ON_ERROR(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &WifiHandler::eventHandler,
                                                        this,
                                                        &instance_any_id),
                                                        LOG_TAG, "wifi event handle register");


    ESP_RETURN_ON_ERROR(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &WifiHandler::eventHandler,
                                                        this,
                                                        &instance_got_ip),
                                                        LOG_TAG, "ip event handle register");                                    

    wifi_config_t wifi_config = {}; 
        
    memcpy(wifi_config.sta.ssid, ssid.c_str(), ssid.size());
    memcpy(wifi_config.sta.password, pass.c_str(), pass.size());    

    ESP_RETURN_ON_ERROR(esp_wifi_set_mode(WIFI_MODE_STA), LOG_TAG, "set mode");
    ESP_RETURN_ON_ERROR(esp_wifi_set_config(WIFI_IF_STA, &wifi_config), LOG_TAG, "set config");
    
    ESP_RETURN_ON_ERROR(esp_wifi_start(), LOG_TAG, "wifi start");

    LOG_INFO("wifi_init_sta finished.");

    return this->waitForConnection();

}

esp_err_t WifiHandler::waitForConnection() {

    EventBits_t bits = xEventGroupWaitBits(m_wifiEventGroup,
        WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
        pdFALSE,
        pdFALSE,
        pdMS_TO_TICKS(30000)); // 30s

    if (bits & WIFI_CONNECTED_BIT) {
        LOG_INFO("connected to ap SSID:%s", m_ssid.c_str());
        return ESP_OK;
    } else if (bits & WIFI_FAIL_BIT) {
        LOG_INFO("Failed to connect to SSID:%s, password:%s", m_ssid.c_str(), m_pass.c_str());
        return ESP_ERR_WIFI_PASSWORD;
    } else {
        LOG_ERROR("UNEXPECTED EVENT");
    }

    LOG_ERROR("Przekroczono czas oczekiwania (Timeout)");
    return ESP_ERR_TIMEOUT;

}

void WifiHandler::connect(IStorage& storage) {
    esp_err_t errSsid = storage.getNvsValue("ssid", m_ssid);
    esp_err_t errPass = storage.getNvsValue("pass", m_pass);
    esp_err_t err;

    if (errSsid == ESP_OK && errPass == ESP_OK && !m_ssid.empty()) {
        LOG_INFO("Pobrano dane WiFi z Storage. SSID: %s", m_ssid.c_str());
        err = this->connect(m_ssid, m_pass);
    } else {
        err = this->connect(EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
    }

    if (err == ESP_OK) {
        LOG_INFO("Sukces, połączono z wifi");
        return;
    }

    this->handleConnectionFailure(err);

}

void WifiHandler::handleConnectionFailure(esp_err_t err) {
    LOG_ERROR("Problem z nawiązaniem  polączenia wifi");

    switch (err) {
        case ESP_ERR_WIFI_SSID:
            LOG_ERROR("BŁĄD: Nazwa sieci (SSID) jest nieprawidłowa.");
            break;

        case ESP_ERR_WIFI_PASSWORD:
            LOG_ERROR("BŁĄD: Uwierzytelnianie nie powiodło się.");
            break;

        case ESP_ERR_TIMEOUT:
            LOG_ERROR("BŁĄD: Przekroczono czas oczekiwania.");
            break;

        case ESP_FAIL:
            LOG_ERROR("BŁĄD: Ogólny błąd (ESP_FAIL).");
            break;

        default:
            LOG_ERROR("BŁĄD: Nieoczekiwany problem: %s", esp_err_to_name(err));
            break;
    }

    // LOG_WARNING("Restart systemu");
    // vTaskDelay(pdMS_TO_TICKS(3000));
    // esp_restart();

}