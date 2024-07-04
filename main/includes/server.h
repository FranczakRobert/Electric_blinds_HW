#ifndef SERVER
#define SERVER

#include "esp_netif.h"
#include "esp_http_server.h"
#include "cJSON.h"

static esp_err_t hello_get_handler(httpd_req_t *req);
esp_err_t post_handler(httpd_req_t *req);
void start_http_server(void);
esp_err_t options_handler(httpd_req_t *req);

static const httpd_uri_t post_uri = {
    .uri       = "/postValue",
    .method    = HTTP_POST,
    .handler   = post_handler,
    .user_ctx  = NULL
};

static const httpd_uri_t options_uri = {
    .uri       = "/postValue",
    .method    = HTTP_OPTIONS,
    .handler   = options_handler,
    .user_ctx  = NULL
};

#endif 