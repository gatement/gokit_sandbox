#include "protocol.h"

extern protocol_cmd_error_t               m_protocol_cmd_error;

/*******************************************************************************
* Function Name  : ExchangeBytes
* Description    : Simulate htons() or ntohs()
* Input          : short int
* Output         : None
* Return         : The short int after exchaged
* Attention      : None
*******************************************************************************/
short ExchangeBytes(short value)
{
    short       tmp_value;
    uint8_t     *index1, *index2;
    
    index1 = (uint8_t *)&tmp_value;
    index2 = (uint8_t *)&value;
    
    *index1 = *(index2 + 1);
    *(index1+1) = *index2;
    
    return tmp_value;
}

/*******************************************************************************
* Function Name  : CheckSum
* Description    : Calculate checksum
* Input          : buf - data address, len - data len
* Output         : None
* Return         : Checksum value
* Attention      : None
*******************************************************************************/
uint8_t CheckSum(uint8_t *buf, int packLen)
{
    int      i;
    uint8_t  sum;

    if(buf == NULL || packLen <= 0) return 0;

    sum = 0;
    for(i = 2; i < packLen-1; i++) sum += buf[i];

    return sum;
}

/*******************************************************************************
* Function Name  : SendErrorAck
* Description    : Send error ack for the request message
* Input          : error_no -  the error code, sn -  the error message sn
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void SendErrorAck(uint8_t error_no, uint8_t sn)
{
    m_protocol_cmd_error.header.sn = sn;
    m_protocol_cmd_error.error = error_no;
    m_protocol_cmd_error.sum = CheckSum((uint8_t *)&m_protocol_cmd_error, sizeof(protocol_cmd_error_t));
    
    SendToUart((uint8_t *)&m_protocol_cmd_error, sizeof(protocol_cmd_error_t), 0);
}

/*******************************************************************************
* Function Name  : SendToUart
* Description    : Send data to uart, waiting for the message ack if the param ack is set
* Input          : buf - data address, packLen - package length, ack - if need to wait for the message ack
* Output         : None
* Return         : None
* Attention      : If param ack is set and doesn't receive message ack, resend package(max 3 times). If data contains 0xFF(except the header), send 0x55 right after 0xFF 
*******************************************************************************/
void SendToUart(uint8_t *buf, uint16_t packLen, uint8_t ack)
{
    uint16_t            i;
    //int                 Send_num;
    //pro_headPart        send_headPart;  
    //pro_commonCmd       recv_commonCmd;
    
    for(i = 0; i < packLen; i++)
    {
        UART1_Send_DATA(buf[i]);
        // If data contains 0xFF(except the header), send 0x55 right after the 0xFF
        if(i >= 2 && buf[i] == 0xFF) UART1_Send_DATA(0x55);
    }
    
    // If ack == 0, no need to wait for ack
    if(ack == 0) return;

    /*
    memcpy(&send_headPart, buf, sizeof(pro_headPart));
    memset(&recv_commonCmd, 0, sizeof(pro_commonCmd));
    
    wait_ack_time = 0;
    Send_num = 1;
    
    while(Send_num < MAX_SEND_NUM)
    {
        if(wait_ack_time < MAX_SEND_TIME)
        {
            if(get_one_package)
            {               
                get_one_package = 0;
                memcpy(&recv_commonCmd, uart_buf, sizeof(pro_commonCmd));
                
                //只有当sn和ACK均配对时，才表明发送成功；
                if((send_headPart.cmd == CMD_SEND_MODULE_P0 && recv_commonCmd.head_part.cmd == CMD_SEND_MODULE_P0_ACK) &&
                    (send_headPart.sn == recv_commonCmd.head_part.sn)) break;
                
                if((send_headPart.cmd == CMD_SET_MODULE_WORKMODE && recv_commonCmd.head_part.cmd == CMD_SET_MODULE_WORKMODE_ACK) &&
                    (send_headPart.sn == recv_commonCmd.head_part.sn)) break;
                
                if((send_headPart.cmd == CMD_RESET_MODULE && recv_commonCmd.head_part.cmd == CMD_RESET_MODULE_ACK) &&
                    (send_headPart.sn == recv_commonCmd.head_part.sn)) break;
            }
        }
        else 
        {
              wait_ack_time = 0 ;
              for(i=0;i<packLen;i++)
              {
                   UART1_Send_DATA(buf[i]);
                   if(i >=2 && buf[i] == 0xFF) UART1_Send_DATA(m_55);
              }
              Send_num ++;        
        }   
    }   
    */
}
