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

    GPIO_PORTB_AFSEL_R &= 0xF7; //Disable alternate function
    GPIO_PORTB_PCTL_R |= 0x7000; //pin 3 config (unsure about this one)
    GPIO_PORTB_DEN_R |= 0x8; //pin 3 set to digital
    GPIO_PORTB_DIR_R |= 0x08; //pin 3 set as an output


    //Timers are for part 2 (should be set up correctly) just need the interrupt handler coded

//    TIMER3_CTL_R &= ~TIMER_CTL_TBEN; //disable timer3b for setup
//    TIMER3_CFG_R = 0x4; //timers are split into 2 16-bit timers
//    TIMER3_TBMR_R = 0x07; //timer 3b mode is set (count down, edge-time, capture)
//    TIMER3_CTL_R |= 0xC00; //both edges trigger timer 3b interrupt (im pretty sure this is correct???)
//    TIMER3_TBILR_R = 0xFFFF; //timer loaded with initial count down num
//    TIMER3_IMR_R |= 0x400; //interrupt mask set for timer 3b
//    TIMER3_CTL_R |= 0x100; //enable the timer 3b
//
//    IntRegister(INT_TIMER3B, TIMER3B_handler); //Handler not coded yet

}

int ping_read() { //Got a little exited I guess
    //read the ping values like adc (for part 2)
    //Timer already masked
//    GPIO_PORTB_AFSEL_R &= 0xF7; // Preserve all other bits while setting pin 3 to GPIO mode
//
//    //Configure pin 3 as output
//    GPIO_PORTB_DIR_R |= 0x08;
//
//    //Wait 5 micro-seconds
//
//    //Configure pin 3 to input
//    GPIO_PORTB_DIR_R |= 0x00;
//
//    //Capture mode after sending input trigger
//
//    //Clear interrupt timer
//    TIMER3_ICR_R &= 0x000;  //Got from slides, not sure why its here
//
//    //Un-mask interrupt timer
//    TIMER3_IMR_R &= 0xCFF;
//
//    //Catch rising edge, store TIMER3_TBR_R in var
//    // int start = TIMER3_TBR_R //Defiantly not exactly correct
//
//    //Catch falling edge, store TIMER3_IMR_R in var
//    // int end = TIMER3_TBR_R //Defiantly not exactly correct
//
//    // Return difference
//    // return (start - end) * delta?; //This could be wrong


    return 1;
}

void TIMER3B_handler() {
    // Handles ping interrupts
}
