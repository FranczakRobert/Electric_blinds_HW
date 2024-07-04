#include "wifi.h"
#include "server.h"
#include "stepper.h"


void app_main(void) {
    wifi_init_sta();
    start_http_server();
    start_motor();
}