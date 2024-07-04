#ifndef STEPPER
#define STEPPER

static void gpio_init();
static void step_motor(int in1, int in2, int in3, int in4);
void start_motor();

#endif