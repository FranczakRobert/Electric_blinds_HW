#ifndef SERVER
#define SERVER

#include "esp_netif.h"
#include "esp_http_server.h"
#include "cJSON.h"

static esp_err_t hello_get_handler(httpd_req_t *req);
esp_err_t post_handler(httpd_req_t *req);
void start_http_server(void);


static const httpd_uri_t get = {
    .uri       = "/hello",
    .method    = HTTP_GET,
    .handler   = hello_get_handler,
    .user_ctx  = NULL
};

static const httpd_uri_t post = {
    .uri       = "/postValue",
    .method    = HTTP_POST,
    .handler   = post_handler,
    .user_ctx  = NULL
};

#endif 