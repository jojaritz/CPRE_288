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
//#include "movement.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

//#warning "Possible unimplemented functions"
#define DELTA_THRESHOLD .4f
#define M_PI 3.14159265358979323846

typedef struct {
    int id;
    int start_angle;
    int end_angle;
    int center_angle;
    float distance;
    int radial_width;
    float linear_width;
} Object;

Object detected_objects[10]; // Array to hold detected objects
int object_count = 0;

// void object_detect(cyBOT_Scan_t *distance, int current_angle);
void object_detect(float distance, int current_angle);
void cyBot_send_string(const char *str);
void print_detected_objects(void);

//int main(void) {
//
//
//    cyBot_uart_init();
//
//    cyBOT_init_Scan(0b0011);
//    timer_init();
//    lcd_init();
//
//
//    // make sure to find calibration value first, different for each cyBOT
//    right_calibration_value = 274750;
//    left_calibration_value = 1251250;
//
//    cyBOT_Scan_t currentScan;
//    currentScan.sound_dist = 0;
//    currentScan.IR_raw_val = 0;
//
//   //cyBOT_Scan_t *currentScanPtr = &currentScan;
//
//    while(1) {
//        char got_Byte = (char)cyBot_getByte();
//        lcd_printf("%c", got_Byte);
//        if(got_Byte == 'm') {
//
//            // does the 180 degree scan and prints it out to putty(m has already been pressed by the time it gets here)
//
//            int i = 0;
//           // fgets(header, sizeof(header), file);
//            for(i; i<=180; i += 2) {
//                //cyBOT_Scan(i, currentScanPtr);
//
//                object_detect(sensor_data_array[i/2], i);
//
//            }
//            print_detected_objects();
//            //and then compare the different linear widths (used to do angle) differences and print out the smallest width difference to putty
//        }
//
//}
//}

void cyBot_send_string(const char *str) { //same as lab 2, loops through the string to send it
    while (*str != '\0') {
        cyBot_sendByte(*str);
        str++;
    }
}

//void object_detect(cyBOT_Scan_t *distance, int current_angle){
void object_detect(float distance, int current_angle){
    static int on_object = 0;
    static int start_angle = 0;
    static int prev_angle = 0;
    static float prev_dist = -1.0f; // Initialized to invalid value to indicate no previous distance
    static float dist_sum = 0.0f;
    static int sample_count = 0;
    float current_dist = distance;

    // Initialize prev_dist on the very first sample
    if (prev_dist < 0.0f) {
        prev_dist = current_dist;
        prev_angle = current_angle;
        return;
    }

    if (!on_object) {//check if we are currently on an object or not
        // Look for a large enough distance drop to indicate the leading edge of an object
        if ((prev_dist - current_dist) > DELTA_THRESHOLD) {
            on_object = 1;
            start_angle = current_angle;
            dist_sum = current_dist;
            sample_count = 1;
        }
    } else {
        // Currently on an object
        // Look for when distance jumps back up significantly
        if ((current_dist - prev_dist) > DELTA_THRESHOLD) {
            on_object = 0;
            int end_angle = prev_angle; // The object ended at the previous angle
            int radial_w = end_angle - start_angle;

            // check if the angle detected is too small to be an object
            if (radial_w >= 4 && object_count < 10) {
                detected_objects[object_count].id = object_count + 1; //update all object values in the struct
                detected_objects[object_count].start_angle = start_angle;
                detected_objects[object_count].end_angle = end_angle;
                detected_objects[object_count].center_angle = (start_angle + end_angle) / 2;
                detected_objects[object_count].radial_width = radial_w;
                detected_objects[object_count].distance = dist_sum / sample_count;

                float avg_dist = dist_sum / sample_count;   // Calculate the linear width of the detected object
                float theta_rad = (radial_w * M_PI) / 180.0;
                detected_objects[object_count].linear_width = 2.0 * avg_dist * sin(theta_rad / 2.0);

                object_count++;
            }
        } else {
            // Still on the object add all readings
            dist_sum += current_dist;
            sample_count++;
        }
    }

    // Update history for next function call
    prev_dist = current_dist;
    prev_angle = current_angle;
}



void print_detected_objects(void) {
    char out[80];
    cyBot_send_string("Object#   Angle   Distance   Angular Width   Linear Width\n\r");

    int smallest_idx = -1;
    int min_radial = 181; // Initialize to maximum possible width
    float min_linear = 9999.0f;

    int i;
    for (i = 0; i < object_count; i++) {
        sprintf(out, "%-7d   %-5d   %-12.1f   %-16d   %-16.1f\n\r",
                detected_objects[i].id, //goes back and prints all of the detected objects to putty
                detected_objects[i].center_angle,
                detected_objects[i].distance,
                detected_objects[i].radial_width,
                detected_objects[i].linear_width);
        cyBot_send_string(out);

        // if (detected_objects[i].radial_width < min_radial) { //this was the original code, but we want to find the smallest linear width, not the smallest radial width
        //     min_radial = detected_objects[i].radial_width;
        //     smallest_idx = i;
        // }
        if (detected_objects[i].linear_width < min_linear) { //calculates the smallest linear width and then prints out the object number, angle, and width to putty
            min_linear = detected_objects[i].linear_width;
            smallest_idx = i;
        }
    }

    if (smallest_idx != -1) {
        // Print the smallest object's information
        sprintf(out, "\n\rSmallest length Object: #%d at %d deg (Width: %.1f cm)\n\r",
                detected_objects[smallest_idx].id,
                detected_objects[smallest_idx].center_angle,
                detected_objects[smallest_idx].linear_width);
        cyBot_send_string(out);
        cyBOT_Scan_t target_scan;
        cyBOT_Scan(detected_objects[smallest_idx].center_angle, &target_scan);
    } else {
        cyBot_send_string("\n\rNo objects detected.\n\r");
    }
}


