#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_err.h"
#include "nvs_flash.h"
#include "math.h"
#include "led.h"
#include "shutters_Data.h"

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

    const char resp[] = "ESP received value";
    char buffer[50];

    // Parse JSON data
    cJSON *json = cJSON_Parse(content);
    if (json == NULL) {
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }

    cJSON *where = cJSON_GetObjectItem(json, "where");
    cJSON *value = cJSON_GetObjectItem(json, "value");

    if(cJSON_IsString(where) && (where->valuestring != NULL)) {

        if(strcmp(where->valuestring, "LED") == 0) {
            if (cJSON_IsString(value)) {
                if(strcmp(value->valuestring, "1") == 0) {
                    turn_WIFI_led_ON();
                    ledON();
                }
                if (strcmp(value->valuestring, "0") == 0)
                {
                    turn_WIFI_led_OFF();
                    ledOFF();
                }
                sprintf(buffer,"%s: %s",resp,value->valuestring);
                httpd_resp_send(req, buffer, HTTPD_RESP_USE_STRLEN);
            } else {
                httpd_resp_send_500(req);
                cJSON_Delete(json);
                return ESP_FAIL;
            }
        }
        else if (strcmp(where->valuestring, "SHUTTERS_TIME") == 0)
        {
           printf("\n|CZAS ROLET|\n");
           sprintf(buffer,"%s: %s",resp,value->valuestring);
           httpd_resp_send(req, buffer, HTTPD_RESP_USE_STRLEN);
           setTimeForSchutters(value->valuestring);
        }
        else if(strcmp(where->valuestring, "GET_TIME") == 0) {
            printf("\n|GET TIME|\n");
            if(getTimeForShutters() != NULL) {
                sprintf(buffer,"%s: %s",resp,getTimeForShutters());
            }
            else {
                sprintf(buffer,"%s: %s",resp,"NO TIME SET");
            }

            httpd_resp_send(req, buffer, HTTPD_RESP_USE_STRLEN);
        }
        // prinData();
    }

    cJSON_Delete(json);
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


