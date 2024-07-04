#ifndef STEPPER
#define STEPPER

void set_motor_mode(int motor_mode_SET);
void gpio_motor_init();
void step_motor(int in1, int in2, int in3, int in4);
void start_motor();
void motor_init();
#endif