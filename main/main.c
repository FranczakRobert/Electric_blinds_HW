#include <stdio.h>
#include "wifi.h"

#include "esp_err.h"
#include "nvs_flash.h"
#include "nvs.h"

#include <stdio.h>
#include "wifi.h"
#include "server.h"

#include <unistd.h>
#include <limits.h>



void app_main(void) {

    wifi_init_sta();
    start_http_server();
}
