#ifndef _PROTOCOL_H
#define _PROTOCOL_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "hal_uart.h"

// cmd
#define     CMD_GET_MCU_INFO                      0x01
#define     CMD_GET_MCU_INFO_ACK                  0x02
#define     CMD_SEND_MCU_P0                       0x03
#define     CMD_SEND_MCU_P0_ACK                   0x04
#define     CMD_SEND_MODULE_P0                    0x05
#define     CMD_SEND_MODULE_P0_ACK                0x06
#define     CMD_SEND_HEARTBEAT                    0x07
#define     CMD_SEND_HEARTBEAT_ACK                0x08
#define     CMD_SET_MODULE_WORKMODE               0x09
#define     CMD_SET_MODULE_WORKMODE_ACK           0x0A
#define     CMD_RESET_MODULE                      0x0B
#define     CMD_RESET_MODULE_ACK                  0x0C
#define     CMD_REPORT_MODULE_STATUS              0x0D
#define     CMD_REPORT_MODULE_STATUS_ACK          0x0E
#define     CMD_REBOOT_MCU                        0x0F
#define     CMD_REBOOT_MCU_ACK                    0x10
#define     CMD_MODULE_CMD_ERROR_ACK              0x11
#define     CMD_MCU_CMD_ERROR_ACK                 0x12

// sub cmd
#define     SUB_CMD_CONTROL_MCU                   0x01
#define     SUB_CMD_QUERY_STATUS                  0x02
#define     SUB_CMD_QUERY_STATUS_ACK              0x03
#define     SUB_CMD_REPORT_STATUS                 0x04

// error code
#define     ERROR_CHECKSUM                        0x01
#define     ERROR_CMD                             0x02
#define     ERROR_OTHER                           0x03

// device info
#define     GEN_PROTOCOL_VER                      "00000004"
#define     BUZ_PROTOCOL_VER                      "00000002"
#define     HARD_VER                              "GOKIT001"
#define     SOFT_VER                              "00000001"
#define     PRODUCT_KEY                           "6f3074fe43894547a4f1314bd7e3ae0b"
#define     BINDABLE_TIMEOUT                      60

typedef struct _protocol_header_t                     protocol_header_t;
typedef struct _protocol_get_mcu_info_ack_t           protocol_get_mcu_info_ack_t;
typedef struct _protocol_cmd_error_t                  protocol_cmd_error_t;
typedef struct _protocol_heartbeat_ack_t              protocol_heartbeat_ack_t;
typedef struct _protocol_report_module_status_ack_t   protocol_report_module_status_ack_t;
typedef struct _protocol_reboot_mcu_ack_t             protocol_reboot_mcu_ack_t;

extern protocol_cmd_error_t                  m_protocol_cmd_error;
extern protocol_get_mcu_info_ack_t           m_protocol_get_mcu_info_ack;
extern protocol_heartbeat_ack_t              m_protocol_heartbeat_ack;
extern protocol_report_module_status_ack_t   m_protocol_report_module_status_ack;
extern protocol_reboot_mcu_ack_t             m_protocol_reboot_mcu_ack;

__packed struct _protocol_header_t
{
    uint8_t                         head[2];
    short                           len;
    uint8_t                         cmd;
    uint8_t                         sn;
    uint8_t                         flags[2];
};

__packed struct _protocol_get_mcu_info_ack_t
{
    protocol_header_t               header;
    uint8_t                         gen_protocol_ver[8];
    uint8_t                         buz_protocol_ver[8];
    uint8_t                         hard_ver[8];
    uint8_t                         soft_ver[8];
    uint8_t                         product_key[32];
    short                           bindable_time;
    uint8_t                         sum;    
};

__packed struct _protocol_cmd_error_t
{
    protocol_header_t               header;
    uint8_t                         error;
    uint8_t                         sum;    
};

__packed struct _protocol_heartbeat_ack_t
{
    protocol_header_t               header;
    uint8_t                         sum;    
};

__packed struct _protocol_report_module_status_ack_t
{
    protocol_header_t               header;
    uint8_t                         sum;    
};

__packed struct _protocol_reboot_mcu_ack_t
{
    protocol_header_t               header;
    uint8_t                         sum;    
};

short    ExchangeBytes(short value);
uint8_t  CheckSum(uint8_t *buf, int packLen);
void     SendToUart(uint8_t *buf, uint16_t packLen, uint8_t ack);

void     SendErrorAck(uint8_t error_no, uint8_t sn);
void     SendGetMcuInfoAck(uint8_t sn);
void     SendHeartbeatAck(uint8_t sn);
void     SendReportModuleStatusAck(uint8_t sn);
void     SendRebootMcuAck(uint8_t sn);

#endif /*_PROTOCOL_H*/
