/**
 * lab3_template.c
 * 
 * Template file for CprE 288 lab 3
 *
 * @author Zhao Zhang, Chad Nelson, Zachary Glanz
 * @date 08/14/2016
 */

#include "button.h"
#include "Timer.h"
#include "lcd.h"
#include "cyBot_uart.h"  // Functions for communiticate between CyBot and Putty (via UART)
#include "sensor-data.h"   // PuTTy: Buad=115200, 8 data bits, No Flow Control, No Party,  COM1
#include "cyBot_Scan.h"

//#warning "Possible unimplemented functions"
#define REPLACEME 0



int main(void) {

	// YOUR CODE HERE
	
//	int i = 0;
//	printf("[");
//	for (i; i < 46; i++) {
//	    printf("\'%.1f\', ", sensor_data_array[i]);
//	}
//	printf("]");

    cyBot_uart_init();

    cyBOT_init_Scan(0b0011);
    timer_init();
    lcd_init();

    // make sure to find calibration value first, different for each cyBOT
    right_calibration_value = 274750;
    left_calibration_value = 1251250;

    cyBOT_Scan_t currentScan;
    currentScan.sound_dist = 0;
    currentScan.IR_raw_val = 0;

    cyBOT_Scan_t *currentScanPtr = &currentScan;

    while(1) {
        char got_Byte = (char)cyBot_getByte();
        lcd_printf("%c", got_Byte);
        if(got_Byte == 'm') {
            int i = 0;
            lcd_printf("SHFVLHFSBJB");

            // creates the header for user to know what the printed values are
            char infoHeader[25] = " ";
            strcpy(infoHeader, "Degrees   Distance (cm)\n\r");
            int x = 0;
            for(x; x < 25; x++) {
                cyBot_sendByte(infoHeader[x]);
            }

            // does the 180 degree scan and prints it out to putty(m has already been pressed by the time it gets here)
            for(i; i<=180; i += 2) {
                cyBOT_Scan(i, currentScanPtr);
                char distance_to_char[20];
                sprintf(distance_to_char, "%-7d   %.1f\n\r", i, currentScanPtr->sound_dist);
                int j = 0;
                for(j; j < 20; j++) {
                    cyBot_sendByte(distance_to_char[j]);
                }
            }
        }

    }


    // above code is part 2 ^^^ down below is part 3
//    timer_init();
//    lcd_init();
//    cyBOT_init_Scan(0b0011);
//    //cyBOT_SERVO_cal(); //only used to calibrate
//    right_calibration_value = 274750;
//    left_calibration_value = 1251250;
//
//    // 0 degrees value is 274750
//    // 180 degrees value is 1251250 both for cybot 25 (easy to find for any)
//
//    int i = 0;
//    cyBOT_Scan_t scan;
//    for(i; i<=180; i+=2) {
//        cyBOT_Scan(i, &scan);
//    }
//
//    return 0;

}
