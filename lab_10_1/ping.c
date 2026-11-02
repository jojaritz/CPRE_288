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

volatile int rising_time;
volatile int end_time;
volatile int capture_state;

void TIMER3B_handler() {
    int count = 1;
    TIMER3_ICR_R |= 0x400;
    GPIO_PORTB_DATA_R = count;
    count++;
    if (count > 8) {
        count = 1;
    }
}

void ping_init() {
    //write the initialization of registers and timer
    SYSCTL_RCGCTIMER_R |= 0x8; //enable timer 3 clock
    SYSCTL_RCGCGPIO_R |= 0x2; //enable port b clock
    timer_waitMillis(3);

    GPIO_PORTB_DIR_R |= 0x08; //pin 3 set as an output
    GPIO_PORTB_AFSEL_R &= 0xF7; //Disable alternate function
    GPIO_PORTB_PCTL_R &= ~0xF000; //Clear pin 3 configuration
    GPIO_PORTB_PCTL_R |= 0x3000; //pin 3 config (unsure about this one)
    GPIO_PORTB_DEN_R |= 0x08; //pin 3 set to digital
    //GPIO_PORTB_DIR_R |= 0x08; //pin 3 set as an output

    TIMER3_CTL_R &= ~0x100; //disable timer3b for setup
    TIMER3_CFG_R = 0x4; //timers are split into 2 16-bit timers
    TIMER3_TBMR_R = 0x07; //timer 3b mode is set (count down, edge-time, capture)
    TIMER3_CTL_R |= 0xC00; //both edges trigger timer 3b interrupt (im pretty sure this is correct???)
    TIMER3_TBILR_R = 0xFFFF; //timer loaded with initial count down num
    TIMER3_IMR_R |= 0x400; //interrupt mask set for timer 3b
    TIMER3_CTL_R |= 0x100; //enable the timer 3b

    IntRegister(INT_TIMER3B, TIMER3B_handler); //Handler not coded yet



}

int ping_read() { 
    GPIO_PORTB_DIR_R |= 0x08;       // PB3 output
    GPIO_PORTB_DATA_R |= 0xFF;      // PB3 HIGH
    timer_waitMicros(5);
    GPIO_PORTB_DATA_R &= ~0x08;     // PB3 LOW
    GPIO_PORTB_DIR_R &= ~0x08;      // PB3 input

    return 0;
}
