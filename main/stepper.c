#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

// Define GPIO pins connected to L298N IN1, IN2, IN3, IN4
#define GPIO_IN1  14
#define GPIO_IN2  27
#define GPIO_IN3  26
#define GPIO_IN4  25

// Function to initialize GPIO pins
static void gpio_init() {
    gpio_config_t io_conf;
    // Configure GPIO pins for output
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    // Bit mask for the pins, adjust according to your GPIO connections
    io_conf.pin_bit_mask = (1ULL << GPIO_IN1) | (1ULL << GPIO_IN2) | (1ULL << GPIO_IN3) | (1ULL << GPIO_IN4);
    gpio_config(&io_conf);
}

// Function to step the motor in a specified direction
static void step_motor(int in1, int in2, int in3, int in4) {
    gpio_set_level(GPIO_IN1, in1);
    gpio_set_level(GPIO_IN2, in2);
    gpio_set_level(GPIO_IN3, in3);
    gpio_set_level(GPIO_IN4, in4);
    vTaskDelay(10 / portTICK_PERIOD_MS); // Adjust delay as per your motor speed requirements
}

void start_motor() {
    // Initialize GPIOs
    gpio_init();
    
    // Example: Rotate motor clockwise for 200 steps
    for (int i = 0; i < 200; i++) {
        step_motor(1, 0, 1, 0); // Adjust sequence based on your motor and driver
        step_motor(0, 1, 1, 0);
        step_motor(0, 1, 0, 1);
        step_motor(1, 0, 0, 1);
    }

    // Example: Rotate motor counterclockwise for 200 steps
    for (int i = 0; i < 200; i++) {
        step_motor(1, 0, 0, 1);
        step_motor(0, 1, 0, 1);
        step_motor(0, 1, 1, 0);
        step_motor(1, 0, 1, 0);
    }

    // End of program
    vTaskDelete(NULL);
}
