#include <string.h>
#include <stdio.h> 
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "esp_system.h" 
#include "esp_wifi.h" 
#include "esp_log.h" 
#include "nvs_flash.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include <cstring.h>

#include "wifi.h"
#include "led.h"

#define EXAMPLE_ESP_MAXIMUM_RETRY  3

static int s_retry_num = 0;
static const char *TAG = "WIFI";
struct Wifi_Data wifi_Data;

static void read_wifi_data(struct Wifi_Data* wifi_data) {
    FILE *in_file  = fopen("wifiData.txt", "r");
    char line_buffer[50];

    fscanf(in_file,"%s",line_buffer);
    strcpy(wifi_data->name,line_buffer);

    fscanf(in_file,"%s",line_buffer);
    strcpy(wifi_data->pssswd,line_buffer);
}

static void event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id,void *event_data){
    if(event_id == WIFI_EVENT_STA_START){
        printf("WIFI CONNECTING....\n");
    }
    else if (event_id == WIFI_EVENT_STA_CONNECTED) {
        config_pin();
        turn_WIFI_led_ON();
        printf("WiFi CONNECTED\n");
    }
    else if (event_id == WIFI_EVENT_STA_DISCONNECTED) {
        printf("WiFi lost connection\n");
    if(s_retry_num<5) {
            esp_wifi_connect();
            s_retry_num++;
            printf("Retrying to Connect...\n");
        }
    }
    else if (event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        char ip_str[IP4ADDR_STRLEN_MAX];
        esp_ip4addr_ntoa(&event->ip_info.ip, ip_str, IP4ADDR_STRLEN_MAX);
        ESP_LOGI(TAG, "Got IP: %s", ip_str);
    }
}

void wifi_init_sta(void)
{
    turn_WIFI_led_OFF();
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    

    nvs_flash_init();
    ESP_ERROR_CHECK(esp_netif_init()); //network interdace initialization

    ESP_ERROR_CHECK(esp_event_loop_create_default()); //responsible for handling and dispatching events
    esp_netif_create_default_wifi_sta(); //sets up wifi wifi_init_config struct with default values

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT(); //sets up wifi wifi_init_config struct with default values
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));//wifi initialised with dafault wifi_initiation

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &event_handler,
                                                        NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &event_handler,
                                                        NULL));

    read_wifi_data(&wifi_Data);

    printf("Name : %s \n",wifi_Data.name);
    printf("Psswd: %s \n",wifi_Data.pssswd);

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = data[0],
            .password = data[1],
         .threshold.authmode = WIFI_AUTH_WPA2_PSK,
        },
    };

    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
    esp_wifi_connect();
}