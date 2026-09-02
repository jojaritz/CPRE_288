#include "open_interface.h"
#include "Timer.h"
#include "movement.h"


void main(){
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
    int i = 0;
       for(i = 0; i < 4; i++){
           move_forward(sensor_data, 50);
           timer_waitMillis(500);
           turn_clockwise(sensor_data, 90);
               timer_waitMillis(500);
       }


    oi_free(sensor_data);
}
