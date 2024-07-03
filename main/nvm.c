#include "nvs_flash.h"
#include "nvs.h"
#include <stddef.h>
#include "nvm.h"

void saveStringToNVS(const char* namespace, const char* key, const char* value) {
    nvs_handle_t nvsHandle;
    esp_err_t err;

    err = nvs_open(namespace, NVS_READWRITE, &nvsHandle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return;
    }

    err = nvs_set_str(nvsHandle, key, value);
    if (err != ESP_OK) {
        printf("Error (%s) saving value to NVS!\n", esp_err_to_name(err));
    }

    err = nvs_commit(nvsHandle);
    if (err != ESP_OK) {
        printf("Error (%s) committing NVS!\n", esp_err_to_name(err));
    }

    nvs_close(nvsHandle);
}


esp_err_t readStringFromNVS(const char* namespace, const char* key, char* buffer, size_t bufferSize) {
    nvs_handle_t nvsHandle;
    esp_err_t err;

    err = nvs_open(namespace, NVS_READWRITE, &nvsHandle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return err;
    }

    err = nvs_get_str(nvsHandle, key, buffer, &bufferSize);
    switch (err) {
        case ESP_OK:
            printf("Raise-Down shutters time : %s\n", buffer);
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            printf("Value not found in NVS!\n");
            break;
        default:
            printf("Error (%s) reading value from NVS!\n", esp_err_to_name(err));
    }

    nvs_close(nvsHandle);

    return err;
}
