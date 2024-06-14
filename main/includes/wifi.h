#ifndef WIFI
#define WIFI
#include "freertos/FreeRTOS.h"
#include "esp_event.h" 

static void event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id,void *event_data);
void wifi_init_sta(void);
#endif 