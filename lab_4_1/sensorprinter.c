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
    lcd_init();

//    typedef struct{
//        float sound_dist;
//        int IR_raw_val;
//    } cyBOT_Scan_t;

    cyBOT_Scan_t *currentScan;

    while(1) {
        char got_Byte = (char)cyBot_getByte();
//        char data = cyBot_getByte();
//        lcd_printf("Data: %c", data);
        lcd_printf("%c", got_Byte);
        if(got_Byte == 'm') {
            int i = 0;
            lcd_printf("SHFVLHFSBJB");
            for(i;i<=180;i+=2) {
                cyBOT_Scan(i, currentScan);
                char distance_to_char[4] = (char) currentScan->sound_dist;
                int j = 0;
                for(j; j < 4; j++) {
                    cyBot_sendByte(distance_to_char[j]);
                }
            }
        }

    }

}
