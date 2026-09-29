/*
 * clean_data.c
 *
 *  Created on: Sep 22, 2026
 *      Author: jojaritz
 */

#include <stdio.h>
#include <stdlib.h>
#include "sensor-data_8.h"

int main(void) {
    int i = 0;
    for(i = 0; i < 92; i++) {
        printf("%.1f", sensor_data_array2[i]);
    }

}


