#include "infrared.h"

void HandleIREvent(uint8_t state)
{
    if(state)
    {
        printf("infrared event: %i\n", 1);
    }
    else
    {       
        printf("infrared event: %i\n", 0);
    }
}
