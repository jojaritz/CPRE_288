/**
 * lab7_template_extra_help.c
 *
 * Description: This is file is meant for those that would like a little
 *              extra help with formatting their code.
 *
 */

#include "timer.h"
#include "lcd.h"
#include "uart.h"


// Adding global volatile varibles for communcating between 
// your Interupt Service Routine, and your non-interupt code.
volatile  char uart_data;  // Your UART interupt code can place read data here
volatile  char flag;       // Your UART interupt can update this flag
                           // to indicate that it has placed new data
                           // in uart_data                     


void main() //*******All I changed was the receiving parts are now using the uart_data variable instead of current_char. This means that the interrupts are handling receiving comms*******
{

  timer_init();
  lcd_init();
  uart_init();
  uart_interrupt_init();

  char buffer[21] = "";
  char current_char;
  int amountInBuff = 0;

  while(1){
      current_char = uart_receive(); //this stays to show that uart_data is changing even if not explicitly stated in the main, and so we can use it in the transmitting part.
      amountInBuff++;

      if ((amountInBuff == 20) && (uart_data != '\r')){

          lcd_printf(" %c: %d ", uart_data, amountInBuff);
          buffer[amountInBuff - 1] = uart_data;
          buffer[20] = '\0';
          uart_sendChar(current_char);

          timer_waitMillis(1000);
          lcd_clear();
          timer_waitMillis(500);

          lcd_printf("%s", buffer);
          timer_waitMillis(1000);
          amountInBuff = 0;
          uart_sendChar('\r'); // these 2 sendChars dont have to be here but it looks less ugly on putty with them
          uart_sendChar('\n');
          uart_sendStr(buffer);

      } else if (uart_data == '\r') {

        buffer[amountInBuff - 1] = '\0';

        lcd_clear();
        timer_waitMillis(500);
        lcd_printf("%s", buffer);
        timer_waitMillis(1000);
        amountInBuff = 0;
        uart_sendChar('\r');
        uart_sendChar('\n');
        uart_sendStr(buffer);

      } else {

          lcd_printf(" %c: %d ", uart_data, amountInBuff);
          buffer[amountInBuff - 1] = uart_data;
          uart_sendChar(current_char);

      }
  }

}

