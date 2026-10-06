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
    char my_data;       // Variable to get bytes from Client
    char command[100];  // Buffer to store command from Client
    int index = 0;      // Index position within the command buffer

    // Write to LCD so that we know the program is running
    lcd_printf("Running");


    while(running){
        
        //char data = cyBot_getByte();
        //lcd_printf("Data: %c", data);
        index = 0;  // Set index to the beginning of the command buffer
        my_data = cyBot_getByte(); // Get first byte of the command from the Client

        // Get the rest of the command until a newline byte (i.e., '\n') received
        while(my_data != '\n' )
        {
            command[index] = my_data;  // Place byte into the command buffer
            index++;
            my_data = cyBot_getByte(); // Get the next byte of the command
        }

        //command[index] = '\n';  // place newline into command in case one wants to echo the full command back to the Client
        command[index+1] = 0;   // End command C-string with a NULL byte so that functions like printf know when to stop printing

        lcd_printf("Got: %s", command);  // Print received command to the LCD screen
        switch(command[0]){//switch statement to check what the user inputted and call the appropriate function
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
