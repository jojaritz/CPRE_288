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

void ping_init() {
    //write the initialization of registers and timer
    SYSCTL_RCGCTIMER_R |= 0x8; //enable timer 3 clock
    SYSCTL_RCGCGPIO_R |= 0x2; //enable port b clock
    timer_waitMillis(3);

    GPIO_PORTB_AFSEL_R &= 0xF7; //Disable alternate function
    GPIO_PORTB_PCTL_R &= ~0xF000; //Clear pin 3 configuration
    GPIO_PORTB_PCTL_R |= 0x7000; //pin 3 config (unsure about this one)
    GPIO_PORTB_DEN_R |= 0x08; //pin 3 set to digital
    GPIO_PORTB_DIR_R |= 0x08; //pin 3 set as an output




}

int ping_read() { 
    GPIO_PORTB_DATA_R |= 0x08;      // PB3 HIGH
    timer_waitMicros(5);            
    GPIO_PORTB_DATA_R &= ~0x08;     // PB3 LOW

    return 0;
}

void TIMER3B_handler() {

}
