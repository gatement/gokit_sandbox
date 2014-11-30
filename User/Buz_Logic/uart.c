#include "uart.h"

uint8_t    uart_buf[256];    // if the longest msg is bigger than this buffer, please just enlarge the buffer
uint16_t   uart_buf_index; 
uint16_t   uart_msg_len;     // the receiving msg total len
uint8_t    uart_msg_sn;      // the next sn which is used to send msg
uint8_t    uart_got_one_msg; // bool, got one msg or not

void HandleModuleStatus(uint16_t status)
{
    uint8_t rssi = (status >> 6) & 0x07;
    printf("wifi status: 0x%x, RSSI: 0x%x\n", status, rssi);
    LED_RGB_Control(0, rssi*32, 0); // use the green light to indicate wifi RSSI
}

void HandleRebootMcu()
{
    delay_ms(600);
    NVIC_SystemReset();
}

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
            // TODO: just for easy debug to disable the checksum checking
            //SendErrorAck(ERROR_CHECKSUM, protocol_header.sn);
            //return;
        }
				
        switch(protocol_header.cmd)
        {
            case CMD_GET_MCU_INFO:
                //printf("CMD_GET_MCU_INFO\n");
                SendGetMcuInfoAck(protocol_header.sn);
                break;
            case CMD_SEND_HEARTBEAT:
                //printf("CMD_SEND_HEARTBEAT\n");
                SendHeartbeatAck(protocol_header.sn);
                break;
            case CMD_REPORT_MODULE_STATUS:
                //printf("CMD_REPORT_MODULE_STATUS\n");
                SendReportModuleStatusAck(protocol_header.sn);
                HandleModuleStatus(uart_buf[8] * 256 + uart_buf[9]);
                break;
            case CMD_REBOOT_MCU:
                //printf("CMD_REBOOT_MCU\n");
                SendRebootMcuAck(protocol_header.sn);
                HandleRebootMcu();
                break;
            case CMD_SEND_MCU_P0:
                printf("CMD_SEND_MCU_P0\n");
                break;
            default:
                //printf("CMD_UNIDENTIFIED\n");
                SendErrorAck(ERROR_CMD, protocol_header.sn);
                break;
        }
    }
}
