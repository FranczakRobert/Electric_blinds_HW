#ifndef NVM
#define NVM
#include <stddef.h>

void saveStringToNVS(const char* namespace, const char* key, const char* value);
esp_err_t readStringFromNVS(const char* namespace, const char* key, char* buffer, size_t bufferSize);

#endif