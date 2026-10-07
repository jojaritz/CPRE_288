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


void main()
{

  timer_init();
  lcd_init();
  uart_init();

  char buffer[21] = "";
  char current_char;
  int amountInBuff = 0;

  while(1){
      current_char = uart_receive();
      amountInBuff++;

      if ((amountInBuff == 20) && (current_char != '\r')){

          lcd_printf(" %c: %d ", current_char, amountInBuff);
          buffer[amountInBuff - 1] = current_char;
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

      } else if (current_char == '\r') {

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

          lcd_printf(" %c: %d ", current_char, amountInBuff);
          buffer[amountInBuff - 1] = current_char;
          uart_sendChar(current_char);

      }
  }

}

