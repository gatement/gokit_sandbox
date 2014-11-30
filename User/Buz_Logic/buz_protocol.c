#include "buz_protocol.h"

protocol_write_t                   m_protocol_write;
protocol_write_ack_t               m_protocol_write_ack;
protocol_read_t                    m_protocol_read;
protocol_mcu_status_t              m_protocol_mcu_status_current;
protocol_mcu_status_t              m_protocol_mcu_status_last;

uint16_t                           check_status_time;
uint16_t                           report_status_idle_time;

extern uint8_t                     uart_msg_sn;

void BuzProtocolInit()
{
    // m_protocol_write_ack
    memset(&m_protocol_write_ack, 0, sizeof(protocol_write_ack_t));
    m_protocol_write_ack.header.head[0] = 0xFF;
    m_protocol_write_ack.header.head[1] = 0xFF;
    m_protocol_write_ack.header.len = ExchangeBytes(sizeof(protocol_write_ack_t) - 4);
    m_protocol_write_ack.header.cmd = CMD_SEND_MCU_P0_ACK;

    // m_protocol_mcu_status_current
    memset(&m_protocol_mcu_status_current, 0, sizeof(protocol_mcu_status_t));
    m_protocol_mcu_status_current.header.head[0] = 0xFF;
    m_protocol_mcu_status_current.header.head[1] = 0xFF;
    m_protocol_mcu_status_current.header.len = ExchangeBytes(sizeof(protocol_mcu_status_t) - 4);
}

void ReportStatus(uint8_t tag, uint8_t sn)
{
    if(tag == QUERY_STATUS)
    {
        m_protocol_mcu_status_current.header.sn = sn;
        m_protocol_mcu_status_current.header.cmd = CMD_SEND_MCU_P0_ACK;
        m_protocol_mcu_status_current.sub_cmd = SUB_CMD_READ_ACK;
        m_protocol_mcu_status_current.sum = CheckSum((uint8_t *)&m_protocol_mcu_status_current, sizeof(protocol_mcu_status_t));
        SendToUart((uint8_t *)&m_protocol_mcu_status_current, sizeof(protocol_mcu_status_t), 0);
    }
    else if(tag == REPORT_STATUS)
    {
        m_protocol_mcu_status_current.header.sn = uart_msg_sn++;
        m_protocol_mcu_status_current.header.cmd = CMD_SEND_MODULE_P0;
        m_protocol_mcu_status_current.sub_cmd = SUB_CMD_REPORT_STATUS;
        m_protocol_mcu_status_current.sum = CheckSum((uint8_t *)&m_protocol_mcu_status_current, sizeof(protocol_mcu_status_t));
        SendToUart((uint8_t *)&m_protocol_mcu_status_current, sizeof(protocol_mcu_status_t), 1);
    }

    memcpy(&m_protocol_mcu_status_last, &m_protocol_mcu_status_current, sizeof(protocol_mcu_status_t));
}

void HandleModuleStatus(uint16_t status)
{
    uint8_t rssi = (status >> 6) & 0x07;
    //printf("wifi status: 0x%x, RSSI: 0x%x\r\n", status, rssi);
    LED_RGB_Control(0, rssi*32, 0); // use the green light to indicate wifi RSSI
}

void HandleControl(uint8_t *buf)
{
    memcpy(&m_protocol_write, buf, sizeof(protocol_write_t));
    
    // control
    if(m_protocol_write.power)
    {
        LED_RGB_Control(100, 0, 0);
    }
    else
    {
        LED_RGB_Control(0, 0, 0);
    }

    // update m_protocol_mcu_status_current
    m_protocol_mcu_status_current.power = m_protocol_write.power;

    // send ack
    m_protocol_write_ack.header.sn = m_protocol_write.header.sn;
    m_protocol_write_ack.sum = CheckSum((uint8_t *)&m_protocol_write_ack, sizeof(protocol_write_ack_t));
    SendToUart((uint8_t *)&m_protocol_write_ack, sizeof(protocol_write_ack_t), 0);
}

void HandleQueryStatus(uint8_t *buf)
{
    memcpy(&m_protocol_read, buf, sizeof(protocol_read_t));
    ReportStatus(QUERY_STATUS, m_protocol_read.header.sn);
}

void HandleBuzCmd(protocol_header_t protocol_header, uint8_t *buf)
{
    uint8_t sub_cmd = buf[8];
    switch(sub_cmd)
    {
        case SUB_CMD_WRITE:
            //printf("SUB_CMD_WRITE\r\n");
            HandleControl(buf);
            break;
        case SUB_CMD_READ:
            //printf("SUB_CMD_READ\r\n");
            HandleQueryStatus(buf);
            break;
        default:
            //printf("SUB_CMD_UNIDENTIFIED\r\n");
            SendErrorAck(ERROR_OTHER, protocol_header.sn);
            break;
    }
}

void CheckStatus()
{
    int             i, diff;
    uint8_t         *index_new, *index_old;
    
    diff = 0;
    if(check_status_time < 200) return;
        
    check_status_time = 0;
    index_new = (uint8_t *)&(m_protocol_mcu_status_current);
    index_old = (uint8_t *)&(m_protocol_mcu_status_last);
        
    for(i = 0; i < sizeof(protocol_mcu_status_t); i++)
    {
        if(*(index_new+i) != *(index_old+i))
        {
            diff = 1;
            break;
        }
    }
        
    // report status every 10 min or status changed
    if(diff > 0 || report_status_idle_time > 60000)
    {
        ReportStatus(REPORT_STATUS, 0);
        report_status_idle_time = 0;
    }
}
