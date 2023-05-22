#include <stdlib.h>
#include <stdio.h>
#include "bcm2835-1.71/src/bcm2835.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

#define SERVO_PIN 18


// servo part: https://www.circuitden.com/blog/16

void move(int angle, int prev_angle)
{
    int j;
    float hightime = 10.3*(float)angle + 546;
    char c;
    int pulse = (int)abs(((angle - prev_angle)*33)/180) + 10;
    for(j=0; j<pulse; j++)//exucting pulses
    {
        bcm2835_gpio_set(SERVO_PIN);
        bcm2835_delayMicroseconds((int)hightime);
        bcm2835_gpio_clr(SERVO_PIN);
        bcm2835_delayMicroseconds(20000 - (int)hightime);//each pulse is 20ms
    }
}


int main(int argc, char **argv)
{
    int max_angle = atoi(argv[1]);
    int angle_inc = atoi(argv[2]);
    // distance reader part


    // servo part
    if(!bcm2835_init()) return 1;


    bcm2835_gpio_fsel(SERVO_PIN, BCM2835_GPIO_FSEL_OUTP); //set pin 18 as output
    int prev_angle = 0;
    int cur_angle = 0;

    for(;cur_angle < max_angle; cur_angle+=angle_inc)
    {
        move(cur_angle, prev_angle);
        prev_angle = cur_angle; //keeping track of previous angle to later calculate number of pulses
        sleep(1);   // measuring
    }

    printf ("Close Comms\n");
    return (0);
}