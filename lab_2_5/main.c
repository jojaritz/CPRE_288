#include "open_interface.h"
#include "Timer.h"
#include "movement.h"
#include "lcd.h"
#include "cyBot_uart.h"


void main(){

    timer_init(); // Initialize Timer, needed before any LCD screen fucntions can be called
                  // and enables time functions (e.g. timer_waitMillis)

    lcd_init();// Initialize the the LCD screen.  This also clears the screen.


    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
    char test[5] = "test";

    cyBot_uart_init();
    cyBot_sendByte(test);


    //lcd_printf("%d", cyBot_getByte(void));


        timer_waitMillis(500);
    
    oi_setWheels(0,0);
    oi_free(sensor_data);
}
