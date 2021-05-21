#include <Arduino.h>
#include <Altino.h>

// 악세서리 제어
int Acc(unsigned char *data)
{
    unsigned char led = 2;
    unsigned char clocksion = 1;

    if (*data & clocksion)
    {
        Sound(40);
    }

    else if (*data & led)
    {
        Led(0x0002);
        Led(0x0001);
        Led(0x8000);
        Led(0x4000);
    }

    else
    {
        Sound(0);
        Led(0x0000);
    }
    
    

    return 0;
}