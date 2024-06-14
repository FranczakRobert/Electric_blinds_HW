#include <stdio.h>
#include "wifi.h"

#include "esp_err.h"
#include "nvs_flash.h"

#include <stdio.h>
#include "wifi.h"
#include "server.h"

#include <unistd.h>
#include <limits.h>



static void print_working_directory() {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working directory: %s\n", cwd);
    } else {
        perror("getcwd() error");
    }
}
void app_main(void) {
   
   print_working_directory();
    wifi_init_sta();
    // start_http_server();
    printf("Hello World\n");

}
