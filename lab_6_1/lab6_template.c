/**
 * lab6_template.c
 * 
 * Template file for CprE 288 lab 6
 *
 * @author Zhao Zhang, Chad Nelson, Zachary Glanz
 * @date 08/14/2016
 *
 * @author Phillip Jones, updated 6/4/2019
 */

#include "button.h"
#include "timer.h"
#include "lcd.h"

#include "cyBot_uart.h"  // Functions for communiticate between CyBot and Putty (via UART)
                         // PuTTy: Buad=115200, 8 data bits, No Flow Control, No Party,  COM1

#include "cyBot_Scan.h"  // For scan sensors 


#warning "Possible unimplemented functions"
#define REPLACEME 0


// Defined in button.c : Used to communicate information between the
// the interupt handeler and main.
extern volatile int button_event;
extern volatile int button_num;


int main(void) {
	button_init();
	lcd_init();
	
    cyBot_uart_init_clean();  // Clean UART initialization, before running your UART GPIO init code

	// Complete this code for configuring the  (GPIO) part of UART initialization
     SYSCTL_RCGCGPIO_R |= 0x02;
     timer_waitMillis(1);            // Small delay before accessing device after turning on clock
     GPIO_PORTB_AFSEL_R |= 0x03;  
     GPIO_PORTB_PCTL_R &= ~(0x000000FF);     // Force 0's in the disired locations
     GPIO_PORTB_PCTL_R |= 0x000000FF;     // Force 1's in the disired locations
     GPIO_PORTB_DEN_R |= 0x03;
     GPIO_PORTB_DIR_R &= ~(0x000000FF);      // Force 0's in the disired locations
     GPIO_PORTB_DIR_R |= 0x000000FF;      // Force 1's in the disired locataions
    
    (Uncomment ME for UART init part of lab) cyBot_uart_init_last_half();  // Completes the UART device initialization part of configuration
	
	// Initialze scan sensors
    // cyBOT_init_Scan();



	// YOUR CODE HERE
		
	while(1)
	{
	
      // YOUR CODE HERE
  


	
	}
	
}
