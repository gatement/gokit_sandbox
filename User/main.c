#include <stm32f10x.h>
#include "hal_uart.h"
#include "hal_motor.h"
#include "hal_rgb_led.h"
#include "hal_temp_hum.h"
#include "hal_infrared.h"
#include "hal_key.h"
#include "delay.h"
#include "key.h"
#include "uart.h"

uint32_t counter;

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

    counter = 0;
    
    while(1)
    {
        printf("b");
        delay_ms(1000);

        // handler uart msg
        HandleMsg();
        
        // handle key events
        HandleKey();        

        // send mcu status in interval
        //CheckStatus();  
    }               
}
