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

int main(void) {
    timer_init();
    ping_init();
    lcd_init();

    while(1) {
            int ticks = ping_read();
            double pulse_ms = (double)ticks / 80000.0;
            double distance_cm = (pulse_ms * 34.0) / 2.0;

            lcd_clear();
            lcd_printf("Tks:%d Ms:%.2f", ticks, pulse_ms);
            lcd_setCursor(1, 0);  // second line
            lcd_printf("Cm:%.1f Ovf:%d", distance_cm, overflow_count);

            timer_waitMillis(500); // 200–500 ms per lab spec
        }
}
