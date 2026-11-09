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
#include "button.h"

volatile int overflow_count;


int main(void) {
    timer_init();
    servo_init();
    lcd_init();
    button_init();
    // sw1 = move 1 degree
    // sw2 = move 5 degrees
    // sw3 = switch between clock and counterclock
    // sw4 = move to 0 (clockwise) or move to 180 (counterclockwise)

    int turnDir = 0; // 0 = counterclockwise, 1 = clockwise
    float currentDegree = 90.0; // used for logic with sw1 and sw2 (90 is default)
    char message[20] = "counterclockwise"; // initial direction message
    servo_move(currentDegree); //initial degree to start

    while(1)
        {

            uint8_t button_num = button_getButton();
            char current_button = (char)(button_num + '0');

            if(current_button == '4') {
                if (turnDir == 1) {
                    lcd_printf("%d\n%s", servo_move(0.0), message);
                } else {
                    lcd_printf("%d\n%s", servo_move(180.0), message);
                }
            } else if(current_button == '3') {
                if (turnDir == 1) {
                    strcpy(message, "counterclockwise");
                    lcd_printf("now in %s", message);
                    turnDir = 0;
                } else {
                    strcpy(message, "clockwise");
                    lcd_printf("now in %s", message);
                    turnDir = 1;
                }
            } else if(current_button == '2') {
                if (turnDir == 1) {
                    currentDegree -= 5.0;
                } else {
                    currentDegree += 5.0;
                }
                lcd_printf("%d\n%s", servo_move(currentDegree), message);
            } else if(current_button == '1') {
                if (turnDir == 1) {
                    currentDegree -= 1.0;
                } else {
                    currentDegree += 1.0;
                }
                lcd_printf("%d\n%s", servo_move(currentDegree), message);
            }

        }


    //servo_move(265.0); // 265 degrees for some reason is 180 w/ 1 at end of actual seconds
                     // 0 is not 0, stops at ~30 to 45 degrees

    //actual 0 is -50, actual 180 is ~265, actual 90 is 115 for cybot 01 (equation for cybot 1: y = 0.0097x + 0.7361)
}
