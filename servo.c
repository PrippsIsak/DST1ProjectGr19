#include "at91sam3x8.h"
#include "system_sam3x.h"
#include "pins.c"

unsigned int status = 0;
void generalConfig();
void initServo();
int servoVal();
void rotateServo(int deg);

int returnValue = 0;

void initServo()
{
  //pulse width modulation controller, instance ID 36, 36-32 = 4
  *AT91C_PMC_PCER1 = (1<<4);
  //*AT91C_PMC_PCER = (1<<13);
  *AT91C_PIOB_PER = analog8;
  *AT91C_PIOB_PDR = analog8;
  *AT91C_PIOB_ABMR = analog8; // ABSR
  *AT91C_PWMC_ENA = (1<<1);
  *AT91C_PWMC_CH1_CMR = 0x5;//AT91C_PWMC_CPRE_MCK_DIV_32;
  //prescaler*period/MCK = 32
  *AT91C_PWMC_CH1_CPRDR = 52500; // 1ms * 20
  *AT91C_PWMC_CH1_CDTYR =  2625;  //ta reda på tills imon 1*10^-3 * 84*10^6/32
}
int servoVal(){

  return *AT91C_PWMC_CH1_CDTYR;
}
void rotateServo(int deg){
  
  if(deg == 10){
    *AT91C_PWMC_CH1_CDTYUPDR = 1838+(233*9*2);// 180 degees
  }
  else if(deg != 0){
    *AT91C_PWMC_CH1_CDTYUPDR = 1838+(233*deg);
  }
  
}