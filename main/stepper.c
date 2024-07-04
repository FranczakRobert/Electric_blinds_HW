#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include <pthread.h>
#include "stepper.h"

#define GPIO_IN1  14
#define GPIO_IN2  27
#define GPIO_IN3  26
#define GPIO_IN4  25

static int motor_mode;

void gpio_motor_init() {
    gpio_config_t io_conf;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.pin_bit_mask = (1ULL << GPIO_IN1) | (1ULL << GPIO_IN2) | (1ULL << GPIO_IN3) | (1ULL << GPIO_IN4);
    gpio_config(&io_conf);
}

void step_motor(int in1, int in2, int in3, int in4) {
    gpio_set_level(GPIO_IN1, in1);
    gpio_set_level(GPIO_IN2, in2);
    gpio_set_level(GPIO_IN3, in3);
    gpio_set_level(GPIO_IN4, in4);
    vTaskDelay(10 / portTICK_PERIOD_MS);
}

void start_motor() {
    gpio_motor_init();
    while (1) {
        switch (motor_mode)
        {
            case -1:
                step_motor(1, 0, 0, 1);
                step_motor(0, 1, 0, 1);
                step_motor(0, 1, 1, 0);
                step_motor(1, 0, 1, 0);
            break;

            case 1:
                step_motor(1, 0, 1, 0);
                step_motor(0, 1, 1, 0);
                step_motor(0, 1, 0, 1);
                step_motor(1, 0, 0, 1);
            break;

            case 0:
                step_motor(0, 0, 0, 0);
            break;

            default:
                break;
        }
    }
}

void set_motor_mode(int motor_mode_SET) {
    motor_mode = motor_mode_SET;
}

void motor_init() {
    pthread_t thread;
    pthread_create(&thread, NULL, start_motor, NULL);
}