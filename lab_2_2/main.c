#include "open_interface.h"
#include "Timer.h"
#include "movement.h"


void main(){
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
        //this runs 4 times which makes the robot move in a square pattern
           move_forward(sensor_data, 100);


    oi_free(sensor_data);
}
