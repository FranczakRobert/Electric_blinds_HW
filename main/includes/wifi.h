#ifndef WIFI
#define WIFI
#include "freertos/FreeRTOS.h"
#include "esp_event.h" 

struct Wifi_Data {
    char name[15];
    char pssswd[15];
}Wifi_Data;

static void event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id,void *event_data);
static struct void read_wifi_data(struct Wifi_Data* wifi_data)
void wifi_init_sta(void);

#endif 