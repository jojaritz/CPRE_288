/**
 * lab3_template.c
 * 
 * Template file for CprE 288 lab 3
 *
 * @author Zhao Zhang, Chad Nelson, Zachary Glanz
 * @date 08/14/2016
 */

#include "button.h"
#include "Timer.h"
#include "lcd.h"
#include "cyBot_uart.h"  // Functions for communiticate between CyBot and Putty (via UART)
                         // PuTTy: Buad=115200, 8 data bits, No Flow Control, No Party,  COM1

//#warning "Possible unimplemented functions"
#define REPLACEME 0



int main(void) {
	button_init();
	lcd_init();
	cyBot_uart_init();            // Don't forget to initialze the cyBot UART before trying to use it
	timer_init();
	// YOUR CODE HERE
	
	
	while(1)
	{
	
	    uint8_t button_num = button_getButton();
	    char current_button = (char)(button_num + '0');
	    char message[20] = " ";

	    if(current_button == '4') {
	        strcpy(message, "last place :( ");
	    } else if(current_button == '3') {
	        strcpy(message, "2nd loser... ");
	    } else if(current_button == '2') {
	        strcpy(message, "good try :D ");
	    } else if(current_button == '1') {
	        strcpy(message, "you did it! yay ");
	    } else {
	        strcpy(message, "nada ");
	    }

	    lcd_printf("%s", message);

	    int i = 0;
	    for(i; i<20; i++) {
	        cyBot_sendByte(message[i]);
	    }

          timer_waitMillis(500);


	
	}
	

}
