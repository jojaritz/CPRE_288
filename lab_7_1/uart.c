/*
 * uart.c
 *
 *  Created on: Oct 6, 2026
 *      Author: carlos28
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <inc/tm4c123gh6pm.h>
#include "uart.h"
#include "timer.h"


void uart_init(){ // make sure to double check these registers with the skeleton code provided in lab7 page. (no answers but has replace me skeleton)
    SYSCTL_RCGCGPIO_R |= 0b10;
    SYSCTL_RCGCUART_R = 0x02;
    timer_waitMillis(1);                 // Small delay before accessing device after turning on clock

    // rcgc_uart (enable clock) 0x02 <- pretty sure this is no longer needed

    GPIO_PORTB_AFSEL_R |= 0x03;
    GPIO_PORTB_PCTL_R &= 0xFFFFFF00;     // Force 0's in the disired locations
    GPIO_PORTB_PCTL_R |= 0x11;           // Force 1's in the disired locations
    GPIO_PORTB_DEN_R |= 0x03;
    GPIO_PORTB_DIR_R &= ~0x03;           // Force 0's in the disired locations
    GPIO_PORTB_DIR_R |= 0b10;            // Force 1's in the disired locataions

    UART1_CTL_R &= 0xFFFFFFFE;           // disables the uart as we set it up below (below should be correct)

    UART1_IBRD_R = 0x08;
    UART1_FBRD_R = 0x2C;
    UART1_LCRH_R = 0x60;
    UART1_CC_R = 0x0;

    UART1_CTL_R |= 0x1;                  // re-enables the uart
}

void uart_sendChar(char data){

    while((UART1_FR_R & 0x20) != 0){
        //nothing is actually here, just waits while FIFO is full. Then it sends data.
    }

    UART1_DR_R = data;
}

char uart_receive(){
    char data = 0;

    while((UART1_FR_R & 0x10) != 0){
        // nothing here, waits while FIFO is empty
    }

    //From lecture slides, still need to find out what it does, besides receiving
    data = (char)(UART1_DR_R & 0xFF);

    return data;
}

// Code below does not need to be implemented in part 1, I am adding it here for later parts (and incase we DO need it for part 1, like sendStr())

void uart_sendStr(const char *data){//this loops through the string to send it
    int i = 0;
    while(data[i] != '\0'){
        uart_sendChar(data[i]);
        i++;
    }
}

void uart_interrupt_init(){

}

void uart_interrupt_handler(){

}
