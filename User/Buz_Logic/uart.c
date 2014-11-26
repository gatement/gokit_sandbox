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
    uint16_t msg_size;
    protocol_header_t protocol_header;

    if(uart_got_one_msg)
    {
        msg_size = uart_buf_index;
        uart_got_one_msg = 0;
        uart_buf_index = 0;

        memset(&protocol_header, 0, sizeof(protocol_header_t));
        memcpy(&protocol_header, uart_buf, sizeof(protocol_header_t));

        // is checksum failed, return invalid_msg_ack
        if(CheckSum(uart_buf, msg_size) != uart_buf[msg_size - 1])
        {
            SendErrorAck(ERROR_CHECKSUM, protocol_header.sn);
            return;
        }
				
        switch(protocol_header.cmd)
        {
            case CMD_GET_MCU_INFO: 
                printf("CMD_GET_MCU_INFO\n");
                break;
            case CMD_SEND_MCU_P0: 
                printf("CMD_SEND_MCU_P0\n");
                break;
            case CMD_SEND_HEARTBEAT: 
                printf("CMD_SEND_HEARTBEAT\n");
                break;
            case CMD_REPORT_MODULE_STATUS: 
                printf("CMD_REPORT_MODULE_STATUS\n");
                break;
            case CMD_REBOOT_MCU: 
                printf("CMD_REBOOT_MCU\n");
                break;
            case CMD_MCU_CMD_ERROR_ACK: 
                printf("CMD_MCU_CMD_ERROR_ACK\n");
                break;
            default:
                break;
        }
    }
}
