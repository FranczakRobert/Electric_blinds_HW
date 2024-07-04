#include <string.h>
#include "led.h"
#include "cJSON.h"
#include "esp_err.h"
#include "shutters_Data.h"
#include "JSON_VAL.h"
#include "nvm.h"

#include <stdio.h>

esp_err_t readJSON(char* content, char* buffer) {
    const char resp[] = "ESP received value";

    cJSON *json = cJSON_Parse(content);
    if (json == NULL) {
        printf("json IS NULL...\n");
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
                if (strcmp(value->valuestring, LED_OFF) == 0) {
                    turn_WIFI_led_OFF();
                    ledOFF();
                    return ESP_OK;
                }
                sprintf(buffer,"%s: %s",resp,value->valuestring);
            } else {
                cJSON_Delete(json);
                return ESP_FAIL;
            }
        }
        else if (strcmp(where->valuestring, SHUTTERS_TIME_SET) == 0)
        {
           printf("\n|Shutters timer|\n");
           sprintf(buffer,"%s: %s",resp,value->valuestring);
           saveStringToNVS(SHUTTERS_SCHEDULE_NVS,KEY,value->valuestring);
           return ESP_OK;
        }
        else if(strcmp(where->valuestring, SHUTTERS_TIME_SCHEDULE) == 0) {
            printf("\n|GET TIME|\n");
            char NVM_buffer[64];  
            esp_err_t error = readStringFromNVS(SHUTTERS_SCHEDULE_NVS,KEY,NVM_buffer,sizeof(NVM_buffer));

            if(error == ESP_OK) {
                // printf("Reading NVM Success...\n");
                sprintf(buffer,"%s: %s",resp,NVM_buffer);
            }
            else {
                // printf("Reading NVM Success...\n");
                sprintf(buffer,"%s: %s",resp, DEFAULT_TIME);
            }
            return ESP_OK;
        }
        else if (strcmp(where->valuestring, "SHUTTERS_CONTROL") == 0) {
            if(strcmp(value->valuestring, "-1") == 0) {
                printf("ARROW DOWN \n");
            }
            if(strcmp(value->valuestring, "1") == 0) {
                printf("ARROW UP \n");
            }
            if(strcmp(value->valuestring, "0") == 0) {
                printf("ARROW STOP \n");
            }
        }
        

        return ESP_OK;
        cJSON_Delete(json);
    }
    return ESP_OK;
}