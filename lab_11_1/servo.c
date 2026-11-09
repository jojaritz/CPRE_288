/*
 * servo.c
 *
 *  Created on: Nov 3, 2026
 *      Author: carlos28
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "servo.h"
#include "Timer.h"
#include <inc/tm4c123gh6pm.h>

void servo_init() {
    SYSCTL_RCGCTIMER_R |= 0x2; //enable timer 1 clock
    SYSCTL_RCGCGPIO_R |= 0x2; //enable port b clock
    timer_waitMillis(3);

    GPIO_PORTB_DIR_R |= 0x20; //pin 5 set as an output
    GPIO_PORTB_AFSEL_R |= 0x20; //enable alternate function
    GPIO_PORTB_PCTL_R &= ~0xF00000; //Clear pin 5 configuration
    GPIO_PORTB_PCTL_R |= 0x700000; //pin 5 config
    GPIO_PORTB_DEN_R |= 0x20; //pin 5 set to digital

    TIMER1_CTL_R &= ~0x100; //disable timer1b for setup
    TIMER1_CFG_R = 0x4; //timers are split into 2 16-bit timers
    TIMER1_TBMR_R = 0xA; //timer 1b mode is set (count down, PWM, Periodic)
    TIMER1_CTL_R &= 0x0000; //output level is set to not inverted
    TIMER1_TBPR_R = 0x04; //timer 1b pre-scaler set
    TIMER1_TBILR_R = 0xE200; //timer loaded with initial count down num (320000) these 2 together make the period
    TIMER1_TBPMR_R = 0x04; //initial match values set
    TIMER1_TBMATCHR_R = 0xE200; //initial match values set
    TIMER1_CTL_R |= 0x100; //enable timer 1b
}

//This functions only purpose is to set the correct match values according to degree!!
int servo_move(float degrees) {
    //the equation to find ms length from degree is (y/180) + 1 = x, where y is degrees and x is counter value in ms
    //to convert that to ticks, multiply ms by 16,000 (this should be the length of the high)
    //finding the length of the low should be 320,000 - found high value.
    //that low value should be what goes into the MATCH registers (plural because we use a prescaler) ????
    //that low value will be named falling_edge_time

    float millisecondPerDegree = 0.00555555556; // This will get us to how many ms we need
    float actualMS = (millisecondPerDegree * degrees) + 1.0; //how long high should be (in ms)

    float pulse_width = 16000.0 * actualMS; //length of high
    float falling_edge_time = 320000.0 - pulse_width; //length of low
    float duty_cycle = pulse_width / 320000.0; //used in calculations ???

    int match_register = (int) falling_edge_time;

    printf("%f\n", falling_edge_time);
    printf("%f\n", actualMS);

    //this sets the match registers up to the correct value based on falling edge time
    TIMER1_TBPMR_R = (match_register >> 16) & 0xFF;
    TIMER1_TBMATCHR_R = match_register & 0xFFFF;

    return 1;
}
