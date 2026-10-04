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
     SYSCTL_RCGCGPIO_R |= 0b10;
     timer_waitMillis(1);            // Small delay before accessing device after turning on clock
     GPIO_PORTB_AFSEL_R |= 0x03;  
     GPIO_PORTB_PCTL_R &= 0xFFFFFF00;     // Force 0's in the disired locations
     GPIO_PORTB_PCTL_R |= 0x11;     // Force 1's in the disired locations
     GPIO_PORTB_DEN_R |= 0x03;
     GPIO_PORTB_DIR_R &= ~0x03;      // Force 0's in the disired locations
     GPIO_PORTB_DIR_R |= 0b10;      // Force 1's in the disired locataions
    
    //(Uncomment ME for UART init part of lab)
     cyBot_uart_init_last_half();  // Completes the UART device initialization part of configuration
	
	// Initialze scan sensors
     //cyBot_uart_init();

    //cyBot_uart_init();            // Don't forget to initialze the cyBot UART before trying to use it
    timer_init();
    init_button_interrupts();

    cyBOT_init_Scan(0b101);


	right_calibration_value = 274750;
    left_calibration_value = 1251250;
	
	cyBOT_Scan_t currentScan;
	currentScan.IR_raw_val = 0;
	cyBOT_Scan_t *currentScanPtr = &currentScan;

	//int previous_scan = 0;
	//int current_scan = 0;

 //	while(1)
	//{
		

 	   // if(abs(previous_scan - current_scan) > (current_scan*.2) ){

		timer_waitMillis(500);

		cyBOT_Scan(90,currentScanPtr);

		char message[100] = "";

		//message = (char)(currentScanPtr->IR_raw_val);
		int current_scan = currentScanPtr->IR_raw_val;

		lcd_printf("%d", current_scan);

		snprintf(message, sizeof(message), "%d", current_scan);
		
		int i = 0;
	    for(i; i<strlen(message); i++) {
	        cyBot_sendByte(message[i]);

		//TO CONVERT FROM IR RAW, USE THIS EQUATION cm = 22241/(IR_RAW -620.4)

	  //  }
	    //previous_scan = current_scan;

 	  //  }
	    // uint8_t button_num = button_getButton();
	    // char current_button = (char)(button_num + '0');
	    // char message[20] = " ";

	    // if(current_button == '4') {
	    //     strcpy(message, "last place :( ");
	    // } else if(current_button == '3') {
	    //     strcpy(message, "2nd loser... ");
	    // } else if(current_button == '2') {
	    //     strcpy(message, "good try :D ");
	    // } else if(current_button == '1') {
	    //     strcpy(message, "you did it! yay ");
	    // } else {
	    //     strcpy(message, "nada ");
	    // }

	    // lcd_printf("%s", message);

	    // int i = 0;
	    // for(i; i<20; i++) {
	    //     cyBot_sendByte(message[i]);
	    // }

        //   timer_waitMillis(500);


	
	}



	
	
}
