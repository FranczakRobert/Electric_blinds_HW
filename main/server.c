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
#include "nvm.h"
#include "JSON_VAL.h"

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

esp_err_t post_handler(httpd_req_t *req)
{
    char content[100];
    const char resp[] = "ESP received value";
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

    cJSON *json = cJSON_Parse(content);
    if (json == NULL) {
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }

    cJSON *where = cJSON_GetObjectItem(json, JSON_OPERATION_TYPE);
    cJSON *value = cJSON_GetObjectItem(json, JSON_VALUE);

    if(cJSON_IsString(where) && (where->valuestring != NULL)) {

        if(strcmp(where->valuestring, LED) == 0) {
            if (cJSON_IsString(value)) {
                if(strcmp(value->valuestring, LED_ON) == 0) {
                    turn_WIFI_led_ON();
                    ledON();
                }
                if (strcmp(value->valuestring, LED_OFF) == 0)
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
        else if (strcmp(where->valuestring, SHUTTERS_TIME_SET) == 0)
        {
           printf("\n|Shutters timer|\n");
           sprintf(buffer,"%s: %s",resp,value->valuestring);
           httpd_resp_send(req, buffer, HTTPD_RESP_USE_STRLEN);
           saveStringToNVS(SHUTTERS_SCHEDULE_NVS,KEY,value->valuestring);
        }
        else if(strcmp(where->valuestring, SHUTTERS_TIME_SCHEDULE) == 0) {
            printf("\n|GET TIME|\n");
            char NVM_buffer[64];  
            esp_err_t error = readStringFromNVS(SHUTTERS_SCHEDULE_NVS,KEY,NVM_buffer,sizeof(NVM_buffer));

            if(error == ESP_OK) {
                sprintf(buffer,"%s: %s",resp,NVM_buffer);
            }
            else {
                sprintf(buffer,"%s: %s",resp, DEFAULT_TIME);
            }

            httpd_resp_send(req, buffer, HTTPD_RESP_USE_STRLEN);
        }
    }

    cJSON_Delete(json);
    return ESP_OK;
}

void start_http_server(void) {
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_register_uri_handler(server, &post);
    }
}


