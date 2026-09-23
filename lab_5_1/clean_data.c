/*
 * clean_data.c
 *
 *  Created on: Sep 22, 2026
 *      Author: jojaritz
 */

#include <stdio.h>
#include <stdlib.h>
#include "sensor-data_8.h"
#include <math.h>

#define DELTA_ERROR 50.0f
#define ANGLE_ERROR 6

float sensor_data_array_cleaned[91]; 
float sensor_data_array_Error[91];

int main(void) {
    int previous_index = 0;
    float safe_value_left = 0.0f;
    float safe_value_right = 0.0f;
    int constant_dist = 1; 
    int angle_count = 0;
    int i = 0;
    for(i = 0; i < 91; i++) {
        if(constant_dist){
            if(fabs(sensor_data_array[i+1] - sensor_data_array[i]) > DELTA_ERROR) {
                safe_value_left = sensor_data_array[i];
                sensor_data_array_cleaned[i] = sensor_data_array[i];
                sensor_data_array_Error[previous_index] = sensor_data_array[i+1];
                previous_index++;
                constant_dist = 0;
                angle_count += 2;
        } else{
            sensor_data_array_cleaned[i] = sensor_data_array[i];
        }  
        }else{ //enters not constant distance, meaning there is a spike in the data or object
            if(fabs(sensor_data_array[i+1] - sensor_data_array[i]) > DELTA_ERROR) {
                if(angle_count <= ANGLE_ERROR) {
                    safe_value_right = sensor_data_array[i+1];
                    float average_value = (safe_value_left + safe_value_right) / 2.0f;
                    int j = 0;
                    int k = 0;
                    for(j = i - (angle_count/2) + 1; j <= i; j++) {
                        sensor_data_array_cleaned[j] = average_value;
                        sensor_data_array_Error[k] = 0.0f;
                        k++;
                    }
                } else {
                    int j = 0;
                    int k = 0;
                    for(j = i - (angle_count/2) + 1; j <= i; j++) {
                        sensor_data_array_cleaned[j] = sensor_data_array_Error[k];
                        sensor_data_array_Error[k] = 0.0f;
                        k++;
                    }
                }
                

            }else{
                sensor_data_array_Error[previous_index] = sensor_data_array[i];
                previous_index++;
                angle_count += 2;
            }
            constant_dist = 1;
        }

    }
    int j = 0;
    for(j = 0; j < 91; j++) {   
        printf("%.1f\n", sensor_data_array_cleaned[j]);
}
}


