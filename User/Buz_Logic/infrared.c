#include "infrared.h"

void HandleIREvent(uint8_t state)
{
    if(state)
    {
        //printf("infrared event: %i\r\n", 1);
    }
    else
    {       
        //printf("infrared event: %i\r\n", 0);
    }
}
