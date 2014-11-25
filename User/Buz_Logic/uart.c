#include <stdio.h>
#include <stdint.h>
#include "uart.h"

uint8_t    uart_buf[1024]; 
uint16_t   uart_buf_index; 
uint16_t   uart_msg_len; 
uint8_t    uart_msg_sn; 
uint8_t    uart_got_one_msg; 

/*******************************************************************************
 * Function Name  : HandleMsg
 * Description    : The message handler function
 * Input          : None
 * Output         : None
 * Return         : None
 * Attention      : None
 *******************************************************************************/
void HandleMsg(void)
{
    if(uart_got_one_msg)
    {
        uart_got_one_msg = 0;
        uart_buf_index = 0;
    }
}
