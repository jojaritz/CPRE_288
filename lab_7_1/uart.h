/*
 * uart.h
 *
 *  Created on: Oct 6, 2026
 *      Author: carlos28
 */

#ifndef UART_H_
#define UART_H_

void uart_init(void);

void uart_sendChar(char data);

char uart_receive(void);



#endif /* UART_H_ */
