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

volatile int overflow_count;


int main(void) {
    timer_init();
    ping_init();
    lcd_init();

    while(1) {
            int ticks = ping_read();
            double pulse_ms = (double)ticks / 16000.0;
            double distance_cm = (pulse_ms * 34.0) / 2.0;

            lcd_clear();
            lcd_printf("Ticks: %d\nTime: %.2f ms\nDist: %.1fcm\nOvf: %d", ticks, pulse_ms, distance_cm, overflow_count);

            timer_waitMillis(500);
        }
}
