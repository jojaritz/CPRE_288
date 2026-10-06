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


void uart_init(){
    SYSCTL_RCGCGPIO_R |= 0b10;
    SYSCTL_RCGCUART_R = 0x01;
    timer_waitMillis(1);            // Small delay before accessing device after turning on clock
    // rcgc_uart (enable clock) 0x02

    GPIO_PORTB_AFSEL_R |= 0x03;
    GPIO_PORTB_PCTL_R &= 0xFFFFFF00;     // Force 0's in the disired locations
    GPIO_PORTB_PCTL_R |= 0x11;     // Force 1's in the disired locations
    GPIO_PORTB_DEN_R |= 0x03;
    GPIO_PORTB_DIR_R &= ~0x03;      // Force 0's in the disired locations
    GPIO_PORTB_DIR_R |= 0b10;      // Force 1's in the disired locataions

    UART1_CTL_R &= 0xFFFFFFFE;

    UART1_IBRD_R = 0x08;
    UART1_FBRD_R = 0x2C;
    UART1_LCRH_R =

    //
}

void uart_sendChar(char data){



}
