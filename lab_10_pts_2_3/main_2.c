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
    while(1) {
        int ticks = ping_read();        // get pulse width in clock cycles
        lcd_printf("Ticks: %d", ticks); // display on LCD
        timer_waitMillis(500);          // measure every 200–500 ms (lab spec)
    }
}
