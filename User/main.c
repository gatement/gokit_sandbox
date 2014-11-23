#include <stm32f10x.h>
#include "hal_uart.h"
#include "hal_motor.h"
#include "hal_rgb_led.h"
#include "delay.h"

int main(void)
{
    // init sys
    SystemInit();

    // init peripheral
    UART_Configuration();
    delay_init();   
    Motor_Init();
    RGB_LED_Init();
    //KEY_GPIO_Init();
    //TIM3_Int_Init(100,7199);
    //DHT11_Init();
    
    while(1)
    {
       	printf("a");
       	printf("about:4\r\n");
        delay_ms(1000);

        //���������ڴ��ڵ�����֡
        //MessageHandle();
        
        //���������ڰ������¼�
        //KeyHandle();        

        //���ϵͳ����״̬
        //CheckStatus();  
    }               
}
