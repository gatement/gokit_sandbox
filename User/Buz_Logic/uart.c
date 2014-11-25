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
    protocol_header_t protocol_header;
    memset(&protocol_header, 0, sizeof(protocol_header_t));

    if(uart_got_one_msg)
    {
        uart_got_one_msg = 0;
        uart_buf_index = 0;

        // is checksum failed, return invalid_msg_ack
        if(CheckSum(uart_buf, uart_buf_index) != uart_buf[uart_buf_index - 1])
        {
            SendErrorAck(ERROR_CHECKSUM, protocol_header.sn);
            return;
        } 

        memcpy(&protocol_header, uart_buf, sizeof(protocol_header_t));

        switch(protocol_header.cmd)
        {
            default:
                break;
        }
    }
}
