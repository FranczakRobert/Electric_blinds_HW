#include "driver/gpio.h"


#define GPIO_OUTPUT_PIN GPIO_NUM_2   // Example GPIO pin

void config_pin() {
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;       // Disable interrupt
    io_conf.mode = GPIO_MODE_OUTPUT;             // Set as output mode
    io_conf.pin_bit_mask = (1ULL << GPIO_OUTPUT_PIN); // Bit mask of the pins to set (e.g., GPIO2)
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE; // Disable pull-down mode
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;     // Disable pull-up mode

    gpio_config(&io_conf);
}

void turn_WIFI_led_ON() {
    gpio_set_level(GPIO_OUTPUT_PIN,1);
}

void turn_WIFI_led_OFF() {
    gpio_set_level(GPIO_OUTPUT_PIN,0);
}