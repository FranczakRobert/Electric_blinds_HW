#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_err.h"
#include "nvs_flash.h"
#include "math.h"
#include "led.h"

#include "server.h"

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

static esp_err_t hello_get_handler(httpd_req_t *req) {
    const char* resp_str = "Hello, World!";
    httpd_resp_send(req, resp_str, strlen(resp_str));
    return ESP_OK;
}

esp_err_t post_handler(httpd_req_t *req)
{
    char content[100];
    size_t recv_size = min(req->content_len, sizeof(content));

    int ret = httpd_req_recv(req, content, recv_size);
    if (ret <= 0) {
        if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
            httpd_resp_send_408(req);
        }
        return ESP_FAIL;
    }

    content[recv_size] = '\0';

    // Parse JSON data
    cJSON *json = cJSON_Parse(content);
    if (json == NULL) {
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }

    cJSON *value = cJSON_GetObjectItem(json, "value");
    if (cJSON_IsNumber(value)) {
        printf("Received value: %d\n", value->valueint);
        if(value->valueint == 1) {
            turn_WIFI_led_ON();
        }
        if (value->valueint == 0)
        {
            turn_WIFI_led_OFF();
        }
        
    } else {
        httpd_resp_send_500(req);
        cJSON_Delete(json);
        return ESP_FAIL;
    }

    cJSON_Delete(json);
    
    const char resp[] = "ESP received value";
    char buffer[50];
    sprintf(buffer,"%s: %d",resp,value->valueint);
    httpd_resp_send(req, buffer, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

void start_http_server(void) {
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_register_uri_handler(server, &get);
        httpd_register_uri_handler(server, &post);
    }
}


