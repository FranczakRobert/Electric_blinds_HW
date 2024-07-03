#include "wifi.h"
#include "server.h"


void app_main(void) {
    wifi_init_sta();
    start_http_server();
}
