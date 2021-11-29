#include "at91sam3x8.h"
#include "system_sam3x.h"
#include "pins.c"

void initKeypad();
int keypad();

void initKeypad()
{
  *AT91C_PIOD_PER = pin28;
  *AT91C_PIOD_PER = pin27;

    *AT91C_PIOC_PER = pin34;
    *AT91C_PIOC_PER = pin35;
    *AT91C_PIOC_PER = pin36;
    *AT91C_PIOC_PER = pin37;
    *AT91C_PIOC_PER = pin39;
    *AT91C_PIOC_PER = pin40;
    *AT91C_PIOC_PER = pin41;

    *AT91C_PMC_PCER = (1<<14);
    *AT91C_PMC_PCER = (1<<13);

    *AT91C_PIOD_PPUDR = pin27;
    *AT91C_PIOD_PPUDR = pin28;

    *AT91C_PIOC_PPUDR = pin34;
    *AT91C_PIOC_PPUDR = pin35;
    *AT91C_PIOC_PPUDR = pin36;
    *AT91C_PIOC_PPUDR = pin37;
    *AT91C_PIOC_PPUDR = pin39;
    *AT91C_PIOC_PPUDR = pin40;
    *AT91C_PIOC_PPUDR = pin41;
  
}
int keypad()
{
  
  *AT91C_PIOC_CODR = pin34;
  //Make colmun pins output
  *AT91C_PIOC_OER = pin39;
  *AT91C_PIOC_OER = pin40;
  *AT91C_PIOC_OER = pin41;

  //Set column pins to high
  *AT91C_PIOC_SODR = pin39;
  *AT91C_PIOC_SODR = pin40;
  *AT91C_PIOC_SODR = pin41;

  //row pins to input
  *AT91C_PIOC_ODR = pin37;
  *AT91C_PIOC_ODR = pin36;
  *AT91C_PIOC_ODR = pin35;
  *AT91C_PIOC_ODR = pin34;


  //Clear OE on octal buss
  *AT91C_PIOD_CODR = pin27;
  *AT91C_PIOD_OER = pin27;
    
  
  /*
pin      key    bit valueal
 37       2          1<<5         
 34       7          1<<2
 35       6          1<<3
 36       4          1<<4  

cols
pin40      3        1<<8
pin41      1        1<<9
pin39      5        1<<7           
*/
  int value = 0;
  int colClear =8; 
 
  for(int col = 0; col<3; col++) //0101  bitwise
  {
    *AT91C_PIOC_CODR = (1<<colClear);
    int startRow = 5;
    for(int row = 0; row<4; row++)
    {
      if(!((*AT91C_PIOC_PDSR)&(1<<startRow)))
      {
        value = row*3 +col+1;
        return value;
      }
      
      if(startRow == 5)
        startRow = 2;
      else
        startRow++;
    }
    
    *AT91C_PIOC_SODR = (1<<colClear); //set column again   8 = 1000, 9 = 1001, 10 = 1010 12 = 1100
    if(colClear == 9)
      colClear = 7; //bitwise 1100
    else
      colClear++; //bitwise 1010 and 1001
  }
 

  //Sets columnpins back to input
  *AT91C_PIOC_ODR = pin39;
  *AT91C_PIOC_ODR = pin40;
  *AT91C_PIOC_ODR = pin41;

  *AT91C_PIOC_SODR = pin27;

  //Enable output to nu mess with screen
  *AT91C_PIOC_OER = pin37;
  *AT91C_PIOC_OER = pin36;
  *AT91C_PIOC_OER = pin35;
  *AT91C_PIOC_OER = pin34;

  *AT91C_PIOC_SODR = pin27;
return value;
}