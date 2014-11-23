#include <stdio.h>
#include "key.h"

uint16_t Key_Return = NO_KEY;

/*******************************************************************************
 * Function Name  : HandleKey
 * Description    : The key handler function
 * Input          : None
 * Output         : None
 * Return         : None
 * Attention      : None
 *******************************************************************************/
void HandleKey(void)
{
    if(Key_Return & PRESS_KEY1)
    {
        if (Key_Return & KEY_LONG)
        {
            printf("key 1 is long pressed\n");
        }
        else if (Key_Return & KEY_DOWN)
        {
            printf("key 1 is pressed down\n");
        }
        else
        {
            printf("key 1 is pressed up\n");
        }

        LED_RGB_Control(0,0,50);
        LED_RGB_Control(0,0,0);

        Key_Return = 0;
    }

    if(Key_Return & PRESS_KEY2)
    {
        if (Key_Return & KEY_LONG)
        {
            printf("key 2 is long pressed\n");
        }
        else if (Key_Return & KEY_DOWN)
        {
            printf("key 2 is pressed down\n");
        }
        else
        {
            printf("key 2 is pressed up\n");
        }

        LED_RGB_Control(0,0,50);
        LED_RGB_Control(0,0,0);

        Key_Return = 0;
    }

    if(Key_Return & PRESS_KEY3)
    {
        if (Key_Return & KEY_LONG)
        {
            printf("key 3 is long pressed\n");
        }
        else if (Key_Return & KEY_DOWN)
        {
            printf("key 3 is pressed down\n");
        }
        else
        {
            printf("key 3 is pressed up\n");
        }

        LED_RGB_Control(0,0,50);
        LED_RGB_Control(0,0,0);

        Key_Return = 0;
    }


    if(Key_Return & PRESS_KEY4)
    {
        if (Key_Return & KEY_LONG)
        {
            printf("key 4 is long pressed\n");
        }
        else if (Key_Return & KEY_DOWN)
        {
            printf("key 4 is pressed down\n");
        }
        else
        {
            printf("key 4 is pressed up\n");
        }

        LED_RGB_Control(0,0,50);
        LED_RGB_Control(0,0,0);

        Key_Return = 0;
    }
}
