#include <stm32f10x.h>
#include "hal_uart.h"
#include "hal_motor.h"
#include "hal_rgb_led.h"
#include "hal_temp_hum.h"
#include "hal_infrared.h"
#include "hal_key.h"
#include "delay.h"
#include "protocol.h"
#include "key.h"
#include "uart.h"

// main vars
uint32_t main_counter;

protocol_cmd_error_t                  m_protocol_cmd_error;
protocol_get_mcu_info_ack_t           m_protocol_get_mcu_info_ack;
protocol_heartbeat_ack_t              m_protocol_heartbeat_ack;
protocol_report_module_status_ack_t   m_protocol_report_module_status_ack;
protocol_reboot_mcu_ack_t             m_protocol_reboot_mcu_ack;

// uart vars
extern uint16_t   uart_buf_index; 
extern uint8_t    uart_msg_sn; 
extern uint8_t    uart_got_one_msg; 

void McuStatusInit()
{
    // main vars
    main_counter = 0;

    // m_protocol_get_mcu_info_ack
    memset(&m_protocol_get_mcu_info_ack, 0, sizeof(protocol_get_mcu_info_ack_t));
    m_protocol_get_mcu_info_ack.header.head[0] = 0xFF;
    m_protocol_get_mcu_info_ack.header.head[1] = 0xFF;
    m_protocol_get_mcu_info_ack.header.len = ExchangeBytes(sizeof(protocol_get_mcu_info_ack_t) - 4);
    m_protocol_get_mcu_info_ack.header.cmd = CMD_GET_MCU_INFO_ACK;
    memcpy(m_protocol_get_mcu_info_ack.gen_protocol_ver, GEN_PROTOCOL_VER, 8);
    memcpy(m_protocol_get_mcu_info_ack.buz_protocol_ver, BUZ_PROTOCOL_VER, 8);
    memcpy(m_protocol_get_mcu_info_ack.hard_ver, HARD_VER, 8);
    memcpy(m_protocol_get_mcu_info_ack.soft_ver, SOFT_VER, 8);
    memcpy(m_protocol_get_mcu_info_ack.product_key, PRODUCT_KEY, 32);
    m_protocol_get_mcu_info_ack.bindable_time = ExchangeBytes(BINDABLE_TIMEOUT);

    // m_protocol_cmd_error
    memset(&m_protocol_cmd_error, 0, sizeof(protocol_cmd_error_t));
    m_protocol_cmd_error.header.head[0] = 0xFF;
    m_protocol_cmd_error.header.head[1] = 0xFF;
    m_protocol_cmd_error.header.len = ExchangeBytes(sizeof(protocol_cmd_error_t) - 4);
    m_protocol_cmd_error.header.cmd = CMD_MODULE_CMD_ERROR_ACK;

    // m_protocol_heartbeat_ack
    memset(&m_protocol_heartbeat_ack, 0, sizeof(protocol_heartbeat_ack_t));
    m_protocol_heartbeat_ack.header.head[0] = 0xFF;
    m_protocol_heartbeat_ack.header.head[1] = 0xFF;
    m_protocol_heartbeat_ack.header.len = ExchangeBytes(sizeof(protocol_heartbeat_ack_t) - 4);
    m_protocol_heartbeat_ack.header.cmd = CMD_SEND_HEARTBEAT_ACK;

    // m_protocol_report_module_status_ack
    memset(&m_protocol_report_module_status_ack, 0, sizeof(protocol_report_module_status_ack_t));
    m_protocol_report_module_status_ack.header.head[0] = 0xFF;
    m_protocol_report_module_status_ack.header.head[1] = 0xFF;
    m_protocol_report_module_status_ack.header.len = ExchangeBytes(sizeof(protocol_report_module_status_ack_t) - 4);
    m_protocol_report_module_status_ack.header.cmd = CMD_REPORT_MODULE_STATUS_ACK;

    // m_protocol_reboot_mcu_ack
    memset(&m_protocol_reboot_mcu_ack, 0, sizeof(protocol_reboot_mcu_ack_t));
    m_protocol_reboot_mcu_ack.header.head[0] = 0xFF;
    m_protocol_reboot_mcu_ack.header.head[1] = 0xFF;
    m_protocol_reboot_mcu_ack.header.len = ExchangeBytes(sizeof(protocol_reboot_mcu_ack_t) - 4);
    m_protocol_reboot_mcu_ack.header.cmd = CMD_REBOOT_MCU_ACK;

    // uart vars
    uart_buf_index = 0;
    uart_msg_sn = 1;
    uart_got_one_msg = 0;
}

int main(void)
{
    // init sys
    SystemInit();

    // init peripheral
    UART_Configuration();
    delay_init();   
    KEY_GPIO_Init();
    TIM3_Int_Init(100,7199);
    Motor_Init();
    RGB_LED_Init();
    DHT11_Init();
    IR_Init();

    McuStatusInit();

    while(1)
    {
        //printf("b");
        //delay_ms(1000);

        // handler uart msg
        HandleMsg();
        
        // handle key events
        HandleKey();        

        // send mcu status in interval
        //CheckStatus();  
    }               
}
