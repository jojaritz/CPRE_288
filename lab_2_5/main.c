#include "open_interface.h"
#include "Timer.h"
#include "movement.h"
#include "lcd.h"
#include "cyBot_uart.h"


void main(){

    timer_init(); // Initialize Timer, needed before any LCD screen fucntions can be called
                  // and enables time functions (e.g. timer_waitMillis)
                  // Initialize the the LCD screen.  This also clears the screen.
    lcd_init();// Initialize the the LCD screen.  This also clears the screen.
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
    cyBot_uart_init();
    int running = 1;
    
    while(running){
        char data = cyBot_getByte();
        lcd_printf("Data: %c", data);

        switch(data){//switch statement to check what the user inputted and call the appropriate function
            case 'w':
                move_forward(sensor_data, 15);
                break;
            case 's':
                move_backward(sensor_data, 15);
                break;
            case 'a':
                turn_counter_clockwise(sensor_data, 45);
                break;
            case 'd':
                turn_clockwise(sensor_data, 45);
                break;
            case 'm':
                uart_sendString("Got an m\r\n");
                break;
            case 'q':
                running = 0;
                break;
        }
    }



    
    oi_setWheels(0,0);
    oi_free(sensor_data);
}
