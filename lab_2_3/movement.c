#include "open_interface.h"

#include "movement.h"


void move_forward(oi_t *sensor, int centimeters){ //this function moves the robot forward a certain amount of centimeters, it takes in the sensor data and the amount of centimeters to move
    int wheel_speed = 250;

    double sum = 0;
    oi_setWheels(wheel_speed, wheel_speed);

    int adj_centimeters = centimeters*10 - 40; //this has a correction factor of 4 centimeters
    while (sum < adj_centimeters){
        oi_update(sensor);
        sum += sensor->distance;
    }

    oi_setWheels(0,0);
}


void turn_clockwise(oi_t *sensor, int degrees){ //This turns clockwise 

    oi_setWheels(-150, 150);


    double sum = 0;

    while((-sum) < (degrees-13)){ //a correction factor of 12 degrees is used to account for overshoot
        oi_update(sensor);
        sum += sensor->angle;
    }

    oi_setWheels(0,0);
}
