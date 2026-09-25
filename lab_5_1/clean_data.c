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

int main(void) {
    const int sensor_data_count = sizeof(sensor_data_array) / sizeof(sensor_data_array[0]);
    int segment_start = -1;
    int i = 0;

    for (i = 0; i < sensor_data_count - 1; i++) {
        if (segment_start < 0) {
            sensor_data_array_cleaned[i] = sensor_data_array[i];

            if (fabs(sensor_data_array[i + 1] - sensor_data_array[i]) > DELTA_ERROR) {
                segment_start = i + 1;
            }
        } else if (fabs(sensor_data_array[i + 1] - sensor_data_array[i]) > DELTA_ERROR) {
            int segment_length = i - segment_start + 1;
            int j = 0;

            if (segment_length * 2 <= ANGLE_ERROR) {
                float average_value = (sensor_data_array[segment_start - 1] + sensor_data_array[i + 1]) / 2.0f;

                for (j = segment_start; j <= i; j++) {
                    sensor_data_array_cleaned[j] = average_value;
                }
            } else {
                for (j = segment_start; j <= i; j++) {
                    sensor_data_array_cleaned[j] = sensor_data_array[j];
                }
            }

            segment_start = -1;
        }
    }

    if (segment_start >= 0) {
        for (i = segment_start; i < sensor_data_count; i++) {
            sensor_data_array_cleaned[i] = sensor_data_array[i];
        }
    } else {
        sensor_data_array_cleaned[sensor_data_count - 1] = sensor_data_array[sensor_data_count - 1];
    }

    for (i = 0; i < sensor_data_count; i++) {
        float value = sensor_data_array_cleaned[i]/100.1;
        printf("%.1f\n", value);
    }

    return 0;
}


