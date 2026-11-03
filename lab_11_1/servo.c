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
    GPIO_PORTB_AFSEL_R &= ~0x20; //Disable alternate function
    GPIO_PORTB_PCTL_R &= ~0xF00000; //Clear pin 5 configuration
    GPIO_PORTB_PCTL_R |= 0x700000; //pin 5 config (unsure about this one)
    GPIO_PORTB_DEN_R |= 0x20; //pin 5 set to digital

    TIMER1_CTL_R &= ~0x100; //disable timer1b for setup
    TIMER1_CFG_R = 0x4; //timers are split into 2 16-bit timers
    TIMER1_TBMR_R = 0xA; //timer 1b mode is set (count down, PWM, Periodic)
    // Rest of the registers not set yet... :(
    TIMER3_CTL_R |= 0xC00; //both edges trigger timer 3b interrupt (im pretty sure this is correct???)
    TIMER3_TBILR_R = 0xFFFF; //timer loaded with initial count down num
    TIMER3_TBPR_R = 0xFF; //timer 3b pre-scaler set
}
