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
#include "lcd.h"
#include <math.h>

//need to be declared for uart to work
volatile char uart_data;
volatile char flag;


double result;
double cleanResult;

double convertToCM(double ir_val);

int main(void) { //currently set up for one scan at a time
    int i = 0;

    timer_init();
    adc_init();
    uart_init();
    lcd_init();

    while (1) {
        char gotByte = uart_receive();

        if (gotByte == 'm') {
            result = adc_read();
            cleanResult = convertToCM(result);
            lcd_printf("Raw: %.3lf\nCM: %d", result, (int)cleanResult);
        }
    }
}

double convertToCM(double ir_val) { //converts raw ir values to cm using the found equation... val = (int) (ir_val - 3821.043636) / -65.128971 <- I found this using an excel graph
    double val = 0.0;
    val = pow((ir_val / 18538), -1.225);
    return val;
}
