#include "at91sam3x8.h"
#include "system_sam3x.h"
#include "head.h"



// Read functions
void initEverything();

int readKey;
float readLight;

int main()
{
  initEverything();
  
    while(1)
  {
      writeTempWhenReady();
    //readLight = calcLight();
    //writeLight(3.3*readLight);
    //delay(10000);
    //    readKey = keypad();
    //  rotateServo(readKey);
    //delay(1000);
  }
  
  /*
  while(1)
  {
    readKey = keypad();
      rotateServo(readKey);
    delay(1000);
  }
*/
  return 0;
  
}

void initEverything(){
    SystemInit();

    
    // Display
    initDisplay();
    dispGraphics();
    clearDisplay();
    
    // Keypad
    //initKeypad();
    //keypad();
    
    // Servo
//    initServo();
    
    // Light
//    initLight();
    
    // Temprature
    SysTick_Config(SystemCoreClock*0.001); //Slow down the clock
    initTemprature();
}



