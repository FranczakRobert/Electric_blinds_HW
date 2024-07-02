#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Shutters_Data {
    unsigned char data;
    char* upDownTime;
}Shutters_Data;

Shutters_Data shutters_Data;

void setTimeForSchutters(char* time) {
    shutters_Data.upDownTime = (char*)malloc(strlen(time) + 1);
    strcpy(shutters_Data.upDownTime, time);
}

char* getTimeForShutters() {
    return shutters_Data.upDownTime;
}

void ledON() {
    shutters_Data.data = shutters_Data.data | 1;
}

void ledOFF() {
    shutters_Data.data = shutters_Data.data & 0;
}

void prinData() {
    printf("Led status: [%d]\n",shutters_Data.data);
    if(shutters_Data.upDownTime != NULL) {
        printf("Time: [%s]\n",shutters_Data.upDownTime);
    }
}