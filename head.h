#include "at91sam3x8.h"
#include "system_sam3x.h"

#include "keypad.c"
#include "display.c"
#include "servo.c"
#include "temprature.c"
#include "lightSensor.c"

void delay(int Value)
{
    int i;
    for(i=0;i<Value;i++)
            asm("nop");
}