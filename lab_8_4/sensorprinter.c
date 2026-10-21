/**
 * lab3_template.c
 * 
 * Template file for CprE 288 lab 3
 *
 * @author Zhao Zhang, Chad Nelson, Zachary Glanz
 * @date 08/14/2016
 */
#include "Timer.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
//#include "sensordata_2.h"
#include "uart.h"
#include "cyBot_Scan.h"
#include "movement.h"


//#warning "Possible unimplemented functions"
#define DELTA_THRESHOLD 40.0f
#define M_PI 3.14159265358979323846
#define DELTA_ERROR 40.0f
#define ANGLE_ERROR 2
#define MAX_DIST_CM 150.0f

typedef struct {
    int id;
    int start_angle;
    int end_angle;
    int center_angle;
    float distance;
    int radial_width;
    float linear_width;
} Object;

Object detected_objects[4]; // Array to hold detected objects
Object smallest_object[1]; // Array holds smallest object for easy access
int object_count = 0;

void object_detect(float distance, int current_angle);
void cyBot_send_string(const char *str);
void print_detected_objects(void);
void clean_data(void);
void autonomous(oi_t *sensor);
void scan_field(cyBOT_Scan_t *currentScanPtr);
void find_direction_to_move(int other_angles[4], int bump_state, io_t *sensor);

float sensor_data_array2_cleaned[91];
float sensor_data_array2[91];

float ir_data_array[91];
float ir_data_array_clean[91];

volatile char uart_data;
volatile char flag;

int isAuto = 0; //will be using to decide when in auto or not, starts off in manual

int main(void) {

    //configure all of the devices
    uart_init();

    cyBOT_init_Scan(0b0111);
    timer_init();
    lcd_init();
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);


    // make sure to find calibration value first, different for each cyBOT
    right_calibration_value = 274750;
    left_calibration_value = 1251250;

    cyBOT_Scan_t currentScan;
    currentScan.sound_dist = 0;
    currentScan.IR_raw_val = 0;

    cyBOT_Scan_t *currentScanPtr = &currentScan;

    char currentMode[50] = " ";
    strcpy(currentMode, "Starting in Manual Mode!\n\r"); //shows user where we start
    cyBot_send_string(currentMode);

    while(1) {

        while(isAuto == 0) {
            char got_Byte = 0;

            if (uart_receive_nonblock(&got_Byte)) {

                if (got_Byte == 't') { //if 't is pressed, the cybot will switch to autonomous mode, might change this to use the uart_data... is more of a rough draft
                    isAuto = 1;
                    char currentMode[50] = " ";
                    strcpy(currentMode, "Switching to Auto Mode!\n\r");
                    cyBot_send_string(currentMode);
                }

                if(got_Byte == 'w') {
                    double distance_left = 5; //change this to however for you wanted to move forward
                    while (distance_left > 0) {
                        double distance_traveled = move_forward(sensor_data, distance_left);
                        distance_left -= distance_traveled;
                        //can we use the go around function in manual?
                        if(sensor_data->bumpLeft && sensor_data->bumpRight){ //if the robot bumps into something, it will go around it
                            go_around(sensor_data, 0); //go around the object in front of it
                            distance_left += 15;
                        }else if(sensor_data->bumpLeft){ //if the robot bumps into something on the left, it will go around it
                            go_around(sensor_data, 0); //go around the object on the right
                            distance_left +=15;
                        }else if(sensor_data->bumpRight){ //if the robot bumps into something on the right, it will go around it
                            go_around(sensor_data, 1); //go around the object on the left
                            distance_left +=15;
                        }
                    }
                } else if(got_Byte == 's') {
                    move_backward(sensor_data, 5);
                } else if(got_Byte == 'a') {
                    turn_counter_clockwise(sensor_data, 20);
                } else if(got_Byte == 'd') {
                    turn_clockwise(sensor_data, 20);
                }else if(got_Byte == 'm') {
                    scan_field(currentScanPtr);
//                    object_count = 0; // Reset the object count before starting a new scan
//                    char infoHeader[50] = " ";
//                    strcpy(infoHeader, "Degrees   Ping(cm)   IR Dist**RAW DATA**\n\r");
//                    cyBot_send_string(infoHeader);
//
//
//                    // does the 180 degree scan and prints it out to putty(m has already been pressed by the time it gets here)
//                    //scan and then run clean data on IR values only
//
//                    int i = 0;
//                    int j = 0;
//                    for(i; i<=180; i += 2) {
//                        for(j = 0; j < 3; j++) {
//                            cyBOT_Scan(i, currentScanPtr);
//                            sensor_data_array2[i/2] += (currentScanPtr->sound_dist)/3.0;
//                            ir_data_array[i/2] += (currentScanPtr->IR_raw_val)/3.0;
//                        }
//
//                        char distance_to_char[50];
//                        sprintf(distance_to_char, "%-7d   %-8.2f   %d\n\r", i, currentScanPtr->sound_dist, currentScanPtr->IR_raw_val);
//                        cyBot_send_string(distance_to_char);
//
//                    }
//
//                    strcpy(infoHeader, "Degrees  IR Dist(cm)**CLEAN DATA**\n\r");
//                    cyBot_send_string(infoHeader);
//
//                    clean_data();
//
//                    i = 0;
//                    for (i = 0; i < 91; i++) {
//                        object_detect(ir_data_array_clean[i], i*2);
//                    }
//                    print_detected_objects();
                }

            }
        }

        while (isAuto == 1) {
            int got_Byte = 0;

            if (uart_receive_nonblock(&got_Byte)) {

                if (got_Byte == 't') { //if 't is pressed, the cybot will switch to manual mode
                    isAuto = 0;
                    char currentMode[50] = " ";
                    strcpy(currentMode, "Switching to Manual Mode!\n\r");
                    cyBot_send_string(currentMode);
                }

                if (got_Byte == 'h') {
                    char get_Byte = 'h';

                    while (get_Byte != 't') {
                        scan_field(currentScanPtr);
//                        object_count = 0; // Reset the object count before starting a new scan
//                        char infoHeader[50] = " ";
//                        strcpy(infoHeader, "Degrees   Ping(cm)   IR Dist**RAW DATA**\n\r");
//                        cyBot_send_string(infoHeader);
//
//                        // does the 180 degree scan and prints it out to putty
//
//                        int i = 0;
//                        int j = 0;
//                        for(i; i<=180; i += 2) {
//
//                            for(j = 0; j < 3; j++) {
//                                cyBOT_Scan(i, currentScanPtr);
//                                sensor_data_array2[i/2] += (currentScanPtr->sound_dist)/3.0;
//                                ir_data_array[i/2] += (currentScanPtr->IR_raw_val)/3.0;
//                            }
//
//                            char distance_to_char[50];
//                            sprintf(distance_to_char, "%-7d   %-8.2f   %d\n\r", i, currentScanPtr->sound_dist, currentScanPtr->IR_raw_val);
//                            cyBot_send_string(distance_to_char);
//                        }
//
//                        strcpy(infoHeader, "Degrees  IR Dist(cm)**CLEAN DATA**\n\r");
//                        cyBot_send_string(infoHeader);
//
//                        clean_data();
//
//                        i = 0;
//                        for (i = 0; i < 91; i++) {
//                            object_detect(ir_data_array_clean[i], i*2);
//                        }
//
//                        print_detected_objects();

                        //what the cyBot plans to do for movement before being asked for another input
                        if (smallest_object[0].distance > 0) { //this works because of a change i made when print detected objects does not detect anything
                            char plannedMove[65] = " ";
                            sprintf(plannedMove, "\n\rI plan to turn to %d degrees and move %.2f cm forward.\n\r", smallest_object[0].center_angle, smallest_object[0].distance);
                            cyBot_send_string(plannedMove);
                        } else {
                            char plannedMove[30] = " ";
                            strcpy(plannedMove, "\n\rI do not plan to move.\n\r");
                            cyBot_send_string(plannedMove);
                        }

                        get_Byte = uart_receive();
                        if (get_Byte == 'h') { //executes planned movement, then it goes back up to scan again and plan another movement
                            autonomous(sensor_data);
                        }
                    }

                    isAuto = 0; //if 't' is hit, the program goes back to manual mode
                    char currentMode[50] = " ";
                    strcpy(currentMode, "Switching to Manual Mode!\n\r");
                    cyBot_send_string(currentMode);
                }

            }
        }
    }


}

















// This function minimizes the code to save space
// Both manual and autonomous scan and can't move while scanning
void scan_field(cyBOT_Scan_t *currentScanPtr) {
    object_count = 0; // Reset the object count before starting a new scan
    char infoHeader[50] = " ";
    strcpy(infoHeader, "Degrees   Ping(cm)   IR Dist**RAW DATA**\n\r");
    cyBot_send_string(infoHeader);

    // does the 180 degree scan and prints it out to putty

    int i = 0;
    int j = 0;


    for(i; i<=180; i += 2) {
        sensor_data_array2[i/2] = 0;
        ir_data_array[i/2] = 0;
        for(j = 0; j < 3; j++) {
            cyBOT_Scan(i, currentScanPtr);
            sensor_data_array2[i/2] += (currentScanPtr->sound_dist)/3.0;
            ir_data_array[i/2] += (currentScanPtr->IR_raw_val)/3.0;
        }
        //do we need the raw vals anymore?
        char distance_to_char[50];
        sprintf(distance_to_char, "%-7d   %-8.2f   %d\n\r", i, currentScanPtr->sound_dist, currentScanPtr->IR_raw_val);
        cyBot_send_string(distance_to_char);
    }

    strcpy(infoHeader, "Degrees  IR Dist(cm)**CLEAN DATA**\n\r");
    cyBot_send_string(infoHeader);

    clean_data();

    i = 0;
    for (i = 0; i < 91; i++) {
        object_detect(ir_data_array_clean[i], i*2);
    }

    print_detected_objects();
}


void cyBot_send_string(const char *str) { //same as lab 2, loops through the string to send it
    while (*str != '\0') {
        uart_sendChar(*str);
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
    cyBOT_Scan_t dist_scan;

    if (current_angle == 0) {
        on_object = 0;
        start_angle = 0;
        prev_angle = 0;
        prev_dist = distance;
        dist_sum = 0.0f;
        sample_count = 0;
        //return;
    }

//    // Initialize prev_dist on the very first sample
//    if (current_angle == 0) {
//        if(distance < 200.0){//checks if the distance is less than 2.0 m, consider it an object
//            on_object = 1;
//             start_angle = current_angle;
//             dist_sum = current_dist;
//             sample_count = 1;
//        }
//        prev_dist = current_dist;
//        prev_angle = current_angle;
//        //return;
//    }
    
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
            if (radial_w >= 4 && object_count < 4) {
                int center_angle = (start_angle + end_angle) / 2;

                //use the sonar for more accureate distance, but use the IR to get the angle and width of the object
                int p = 0;
                float dist = 0.0;
                for (p = 0; p < 4; p++) { //CHANGE TO 3 BUT CHECK IF IT WORKS
                    cyBOT_Scan(center_angle, &dist_scan);
                    dist += dist_scan.sound_dist / 3.0;
                }
                

                detected_objects[object_count].id = object_count + 1; //update all object values in the struct
                detected_objects[object_count].start_angle = start_angle;
                detected_objects[object_count].end_angle = end_angle;
                detected_objects[object_count].center_angle = center_angle;
                detected_objects[object_count].radial_width = radial_w;
                detected_objects[object_count].distance = dist;

                // Calculate the linear width of the detected object
                float theta_rad = (radial_w * M_PI) / 180.0;
                detected_objects[object_count].linear_width = 2.0 * dist * sin(theta_rad / 2.0);

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

    if(current_angle == 180) {//catches object if it is at the edge of the scan range
        if (on_object) {
            int end_angle = prev_angle; // The object ended at the previous angle
            int radial_w = end_angle - start_angle;

            // check if the angle detected is too small to be an object
            if (radial_w >= 4 && object_count < 4) {

                int center_angle = (start_angle + end_angle) / 2;

                int p = 0;
                float dist = 0.0;
                for (p = 0; p < 4; p++) {
                    cyBOT_Scan(center_angle, &dist_scan);
                    dist += dist_scan.sound_dist / 3.0;
                }


                detected_objects[object_count].id = object_count + 1; //update all object values in the struct
                detected_objects[object_count].start_angle = start_angle;
                detected_objects[object_count].end_angle = end_angle;
                detected_objects[object_count].center_angle = center_angle;
                detected_objects[object_count].radial_width = radial_w;
                detected_objects[object_count].distance = dist;

                // Calculate the linear width of the detected object
                float theta_rad = (radial_w * M_PI) / 180.0;
                detected_objects[object_count].linear_width = 2.0 * dist * sin(theta_rad / 2.0);

                object_count++;
            }
        }
    }
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
       float center_rad = detected_objects[smallest_idx].center_angle * M_PI / 180.0;
       int accurate_angle = (int)(atan2(14 + detected_objects[smallest_idx].distance * sin(center_rad),detected_objects[smallest_idx].distance * cos(center_rad)) * (180.0 / M_PI));
       sprintf(out, "\n\rSmallest length Object: #%d at %d deg (Width: %.2f cm), Accurate Angle: %d deg\n\r",
               detected_objects[smallest_idx].id,
               detected_objects[smallest_idx].center_angle,
               detected_objects[smallest_idx].linear_width,
               accurate_angle);
       smallest_object[0] = detected_objects[smallest_idx];
       smallest_object[0].center_angle = accurate_angle;
       cyBot_send_string(out);
       cyBOT_Scan_t target_scan;
       cyBOT_Scan(detected_objects[smallest_idx].center_angle, &target_scan);

   } else {
       cyBot_send_string("\n\rNo objects detected.\n\r");
       smallest_object[0].center_angle = 90; //these make sure that the cybot will not move in auto mode if no objects are detected
       smallest_object[0].distance = 0.0;
   }
}

void clean_data(void) {
    int i = 0;
    int object_state = 0; // 0: not on object, 1: on object
    int last_seg = 0;

    for (i = 0; i < 91; i++) {
        val = MAX_DIST_CM;
        if (ir_data_array[i] > 0) {
            val = pow(ir_data_array[i] / 10066.0, -1.736);
            if (val > MAX_DIST_CM || val != val) val = MAX_DIST_CM;
        }
        ir_data_array_clean[i] = val;
    }
    

    float running_sum = ir_data_array_clean[0];
    int segment_start = 0;
    int count = 1;

    for(i = 1; i < 91; i++) {
        float avg = running_sum / count;
        if(fabs(ir_data_array_clean[i+1] - avg) > DELTA_ERROR) {
            if(fabs(ir_data_array_clean[i+2] - avg) > DELTA_ERROR && i < 90){
                int a;
                for(a = segment_start; a <= i; a++) {
                    ir_data_array_clean[a] = avg;
                }
                segment_start = i + 1;
                running_sum = ir_data_array_clean[segment_start];
                count = 1;
            }
        }else {
            running_sum += ir_data_array_clean[i];
            count++;
        }
    }

        int a;
        float avg = running_sum / count;
        for (a = segment_start; a < 91; a++) ir_data_array_clean[a] = avg;


    for (i = 0; i < 91; i++) {
        //float value = sensor_data_array2_cleaned[i]/100.1;
        //sensor_data_array2_cleaned[i] = value;

        // int val = (int) pow((ir_data_array[i]/10066.0), -1.736);
        // ir_data_array_clean[i] = val;


        char distance_to_char[20];
        sprintf(distance_to_char, "%-7d    %.2f\n\r", i*2, ir_data_array_clean[i];);
        printf("%s\n", distance_to_char); //prints to console
        cyBot_send_string(distance_to_char);

    }

}



void autonomous(oi_t *sensor) {
    // Idea
    // Get angle of smallest object
    int angle = smallest_object[0].center_angle;
    float distance_left = smallest_object[0].distance - 3; // -3 accounts for some error, might change it depending on testing accuracy
    // Turn to face object
    int l;
    int other_angles[4] = {0, 0, 0, 0};
    int bump_state = 0; // 0: no bump, 1: left bump, 2: right bump, 3: both bumps
    for(l=0; l<4; l++){
        if(detected_objects[l].center_angle != angle && detected_objects[l].center_angle != 0){
            other_angles[l] = detected_objects[l].center_angle;
        }
    }
    if (angle > 90) {
        turn_counter_clockwise(sensor, angle - 90); // Turn left
    } else if (angle < 90) {
        turn_clockwise(sensor, 90 - angle); // Turn Right
    }
    // Angle is 90, already faced the correct way
    // Go forward X amount, avoiding obstacles that hit bumpers. if invalid distance (no obj detected), does not move.
    while (distance_left > 0) {
        float distance_traveled = move_forward(sensor, distance_left);
        distance_left -= distance_traveled;


        if(sensor->bumpLeft && sensor->bumpRight){ 
            bump_state = 3; // Both bumpers hit
            find_direction_to_move(other_angles, bump_state, sensor); //go around the object in front of it
            distance_left += 15;
        }else if(sensor->bumpLeft){ //if the robot bumps into something on the left, it will go around it
            bump_state = 1; // Left bumper hit
            find_direction_to_move(other_angles, bump_state, sensor); //go around the object on the right
            distance_left +=15;
        }else if(sensor->bumpRight){ //if the robot bumps into something on the right, it will go around it
            bump_state = 2; // Right bumper hit
            find_direction_to_move(other_angles, bump_state, sensor); //go around the object on the left
            distance_left +=15;
        }
    }

    // Scan again

}

void find_direction_to_move(int other_angles[4], int bump_state, io_t *sensor) {
    // This function will determine the best direction to move based on the angles of other detected objects.
    // It will set the value of *direction to -1 for left, 1 for right, or 0 for straight ahead.

    int i;
    int left_count = 0;
    int right_count = 0;

    for (i = 0; i < 4; i++) {
        if (other_angles[i] != 0) {
            if (other_angles[i] < 90) {
                left_count++;
            } else if (other_angles[i] > 90) {
                right_count++;
            }
        }
    }

    if (left_count > 0 && right_count > 0) {
        if(bump_state == 1) { // If left bumper is hit, move right
            go_around(sensor, 0);
        } else if(bump_state == 2) { // If right bumper is hit, move left
            go_around(sensor, 1);
        } else if(bump_state == 3) { //both bumpers hit, find the furthest object and move in that direction
            int furthest_angle = 0;
            int saved_direction = 0;
            for(i=0; i<4; i++){
                if(fabs(other_angles[i]-90) > furthest_angle && other_angles[i] != 0){
                    furthest_angle = fabs(other_angles[i]-90);
                    saved_direction = (other_angles[i] < 90) ? -1 : 1; // -1 for left, 1 for right
                }
            }
            if(saved_direction == -1){
                go_around(sensor, 1); // Move left
            } else if(saved_direction == 1){
                go_around(sensor, 0); // Move right
            }
        }
    } else if (left_count == 0 && right_count > 0) {
        go_around(sensor, 1); // Move left
    } else if (left_count > 0 && right_count == 0) {
        go_around(sensor, 0); // Move right
    }
    
}
