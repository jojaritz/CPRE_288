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
                cyBot_send_string(distance_to_char);
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

void cyBot_send_string(const char *str) {
    while (*str != '\0') {
        cyBot_sendByte(*str);
        str++;
    }
}

void object_detect(cyBOT_Scan_t *distance, int current_anglevoid ){

    //got to check if distance is changed again to stop counting angle
if (current_dist > 0.0f && current_dist < DIST_THRESHOLD) {
        if (!in_object) {
            // Transition: Open Space -> Object Detected
            in_object = 1;
            start_angle = current_angle;
            dist_sum = 0.0f;
            sample_count = 0;
        }
        // Accumulate distance values to compute the average later
        dist_sum += current_dist;
        sample_count++;
    } else {
        if (in_object) {
            // Transition: Object Ended -> Back to Open Space
            in_object = 0;
            int end_angle = prev_angle;
            int radial_w = end_angle - start_angle;

            // Optional noise check: ignore single-angle spikes (< 4 degrees)
            if (radial_w >= 4 && object_count < MAX_OBJECTS) {
                detected_objects[object_count].id = object_count + 1;
                detected_objects[object_count].start_angle = start_angle;
                detected_objects[object_count].end_angle = end_angle;
                detected_objects[object_count].center_angle = (start_angle + end_angle) / 2;
                detected_objects[object_count].radial_width = radial_w;
                detected_objects[object_count].distance = dist_sum / sample_count;

                object_count++;
            }
        }
    }
    prev_angle = current_angle;//in this function are we returning anything or just the current angle or the smalles angle?
}