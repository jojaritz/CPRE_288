/*
 * main.c
 *
 *  Created on: Oct 22, 2026
 *      Author: carlos28
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "adc.h"
#include "uart.h"
#include "Timer.h"

int main(void) {
    unsigned short results[100];
    int i = 0;

    timer_init();
    adc_init();

    for (i = 0; i < 100; i++) {
        results[i] = adc_read();
        printf("%d ", results[i]);
    }
}
