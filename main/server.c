#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_err.h"
#include "nvs_flash.h"
#include "math.h"

#include "server.h"
#include "jsonHandler.h"

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

esp_err_t post_handler(httpd_req_t *req)
{
    char content[100];
    char buffer[100];
    size_t recv_size = min(req->content_len, sizeof(content));

    int ret = httpd_req_recv(req, content, recv_size);
    if (ret <= 0) {
        if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
            httpd_resp_send_408(req);
        }
        return ESP_FAIL;
    }

    content[recv_size] = '\0';
    esp_err_t err = readJSON(content,&buffer);

    if(err != ESP_OK) {
        httpd_resp_send_500(req);
    }
    else {
        httpd_resp_send(req, buffer, HTTPD_RESP_USE_STRLEN);
    }
    
    return ESP_OK;
}

void start_http_server(void) {
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_register_uri_handler(server, &post);
    }
}


