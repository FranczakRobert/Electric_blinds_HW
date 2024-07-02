#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Shutters_Data {
    unsigned char data;
}Shutters_Data;

Shutters_Data shutters_Data;

void ledON() {
    shutters_Data.data = shutters_Data.data | 1;
}

void ledOFF() {
    shutters_Data.data = shutters_Data.data & 0;
}

void prinData() {
    printf("Led status: [%d]\n",shutters_Data.data);
}