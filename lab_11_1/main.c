/*
 * main.c
 *
 *  Created on: Oct 29, 2026
 *      Author: carlos28
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ping.h"
#include "adc.h"
#include "uart.h"
#include "Timer.h"
#include <inc/tm4c123gh6pm.h>
#include "lcd.h"
#include "servo.h"

volatile int overflow_count;


int main(void) {
    timer_init();
    servo_init();
    lcd_init();


    servo_move(0.0); // 265 degrees for some reason is 180 w/ 1 at end of actual seconds
    timer_waitMillis(20);
}
