#include "at91sam3x8.h"
#include "system_sam3x.h"
#include "pins.c"

float light2 = 0;
int lightFlag = 0;
float light = 0;
float light1 = 0;
int flag1 = 0;
int flag2 = 0;

float calcLight();
void initLight();
void ADC_Handler();

void init();
void ADC_Handler();
void writeLight(float temprature);



void initLight()
{
  *AT91C_PMC_PCER1 = (1<<5); // Instance ID for ADC is 37, usuing the data sheet for pcer and find that PCER1  shift 5 is equal to 37
  //*AT91C_PMC_PCER = (1<<11);//block A
  
  NVIC_ClearPendingIRQ(ADC_IRQn); // Clear pending interrupts
  NVIC_SetPriority(ADC_IRQn,1); // Set priority to 1 
  NVIC_EnableIRQ(ADC_IRQn); // Enable a device specific interrupt
    
  *AT91C_ADCC_MR=(1<<9); //prescaler
  *AT91C_PIOA_PER = analog5; //enable analog 5.
  *AT91C_PIOA_PER = analog6; //enable analog 5.
  //INPUT PÅ BÅDE STÄNG PULL UP
  *AT91C_PIOA_ODR = analog5;
  *AT91C_PIOA_ODR = analog6;
  
  *AT91C_PIOA_PPUDR = analog5;
  *AT91C_PIOA_PPUDR = analog6;
  *AT91C_ADCC_CHER = (1<<1); //offset 0x10 from Register Mapping
  *AT91C_ADCC_CHER = (1<<2);
  *AT91C_ADCC_CWR = (1<<24); //ENABLE THE TAG
  
  *AT91C_ADCC_IER = (1<<1);
  *AT91C_ADCC_IER = (1<<2);
}
float calcLight()
{
  
  *AT91C_ADCC_CR =(0x2<<0); //STARTING control register at channel 1
  
  //while(*AT91C_ADCC_SR&(1<<24) != (1<<24)){}
  if(flag1 == 1)
  {
     light1 = (*AT91C_ADCC_CDR1 & 0xFFF);
     light1 = light1 / 4095;
     *AT91C_ADCC_IER = (1<<1);
     flag1 = 0;
  }
  if(flag2 == 1)
  {
    light2 = (*AT91C_ADCC_CDR2 & 0xFFF);
    light2 = light2 / 4095;
    *AT91C_ADCC_IER = (1<<2);
    flag2 = 0;
  }
   //wanna remove the CHNB and just save the data
  //STEP SIZE = 2^n == 2^10 == 4095 
 
  light = (light1+light2)/2; //converting analog signal to digital
  
  
  return light;    
}

void ADC_Handler()
{
  if((*AT91C_ADCC_SR&(1<<1)) == (1<<1))
  {
    *AT91C_ADCC_IDR = (1<<1);
    flag1 = 1;
  }
  if((*AT91C_ADCC_SR&(1<<2)) == (1<<2))
  {
    *AT91C_ADCC_IDR = (1<<2);
    flag2 = 1;
  }   
}

void writeLight(float temprature)
{
    //tenthDigit =(int)temprature/10; //Gets first digit from number
    int singleDigit=(int)temprature; //gets second digit from nmber
    int decimal=(int)(temprature*10)%10; //gets first decimal digit from number
    int decimal2=(int)(temprature*100)%10;
    int decimal3=(int)(temprature*1000)%10;

    //Set start pos
    Write_Data_2_Display(0x0);
    Write_Data_2_Display(0x0);
    
    Write_Data_2_Display(0x00); // X-axis
    Write_Data_2_Display(0x00); // Y-axis
    Write_Command_2_Display(0x24); // Start position
   
   //printKey(tenthDigit);
    printKey(singleDigit);
    printKey(11);
    printKey(decimal);
    printKey(decimal2);
    printKey(decimal3);
    //printKey(12);
    //printKey(13);
    printKey(14);
    printKey(15);
    
}