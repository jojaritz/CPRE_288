/*
 * ping.c
 *
 *  Created on: Oct 28, 2026
 *      Author: carlos28
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ping.h"
#include "Timer.h"
#include <inc/tm4c123gh6pm.h>

void ping_init() {
    //write the initialization of registers and timer
    SYSCTL_RCGCTIMER_R |= 0x8; //enable timer 3 clock
    SYSCTL_RCGCGPIO_R |= 0x2; //enable port b clock
    timer_waitMillis(3);

    GPIO_PORTB_PCTL_R |= 0x00001000;
    GPIO_PORTB_DEN_R |= 0x8; //not sure what im doing with these yet, just following textbook ^ \/
    GPIO_PORTB_DIR_R |= 0x8;

    TIMER3_CTL_R &= ~TIMER_CTL_TBEN; //disable timer3b for setup
    TIMER3_CFG_R = 0x4; //timers are split into 2 16-bit timers
    TIMER3_TBMR_R |= 0x17; //timer 3b mode is set
    TIMER3_CTL_R |= 0xC00; //both edges trigger timer 3b interrupt (im pretty sure this is correct???)
    TIMER3_TBILR_R = 0xFFFE; //timer loaded with upper bound **0xFFFF breaks something**
    TIMER3_IMR_R |= 0x400; //interrupt mask set for timer 3b
    TIMER3_CTL_R |= 0x100; //enable the timer 3b

    //this function should be done but it does not feel like it
}

int ping_read() {
    //read the ping values like adc
}
