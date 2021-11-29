#include "at91sam3x8.h"
#include "system_sam3x.h"
#include "pins.c"

int bus = 0x3FC;  // All pins from 34 to 41

void Write_Data_2_Display(unsigned char Data);
void Write_Command_2_Display(unsigned char Command);
void dispGraphics(void);
void printKey(int key);
void initDisplay();
void delay();
unsigned char Read_Status_Display(void);


unsigned char Read_Status_Display(void){

    unsigned char temp;

    //make bus input
    *AT91C_PIOC_ODR = bus; //output disable register

    // dir = input
    *AT91C_PIOC_SODR = pin50; //set output data register

    //Clear/enable output 74 chip
    *AT91C_PIOC_CODR = pin51; //clear output data register

    // Set C/D
    *AT91C_PIOC_SODR = pin49; //set output data register

    //Clear chip select display
    *AT91C_PIOC_CODR = pin48; //clear output data register

    //Clear read display
    *AT91C_PIOC_CODR = pin47; //clear output data register

    delay(10);

    //Read data bus and save it in temp
    temp = (unsigned char)(*AT91C_PIOC_PDSR & bus)>>2;

    //Set chip select display
    *AT91C_PIOC_SODR = pin48; //set output data register

    //Set read display
    *AT91C_PIOC_SODR = pin47; //set output data register

    //Disable output (74chip)
    *AT91C_PIOC_SODR = pin51; //set output data register

    // dir = output
    *AT91C_PIOC_CODR = pin50; //clear output data register

    return temp;

}

void Write_Command_2_Display(unsigned char Command){
    // Wait until status returns an OK
    while(1){
        //unsigned char temp = Read_Status_Display();
        if(Read_Status_Display() &(3<<2))  //STA2,3
            break;
    }

    //Read_Status_Display();

    //Enables bus pins TEST
    *AT91C_PIOC_OER = bus;


    // Clear databus
    *AT91C_PIOC_CODR = bus; //clear output data register

    //Set data to databus
    *AT91C_PIOC_SODR = (unsigned int)(Command<<2);

    // Set dir as output (74chip)
    *AT91C_PIOC_CODR = pin50; //clear output data register

    // Enable output (74chip)
    *AT91C_PIOC_CODR = pin51; //clear output data register

    // Set databus as output
    *AT91C_PIOC_OER = bus;

    // Set C/D signal High (1 = Command)
    *AT91C_PIOC_SODR = pin49; //set output data register

    // Clear chip select display
    *AT91C_PIOC_CODR = pin48;//clear output data register

    // Clear write display
    *AT91C_PIOC_CODR = pin46;//clear output data register

    delay(10);

    // Set chip enable display
    *AT91C_PIOC_SODR = pin48; //set output data register

    // Set write display
    *AT91C_PIOC_SODR = pin46; //set output data register

    // Disable output (74chip)
    *AT91C_PIOC_SODR = pin51; //set output data register

    // Make databus as input
    *AT91C_PIOC_ODR = bus; //output disable register


}

void Write_Data_2_Display(unsigned char Data) {
    // Wait until status returns an OK
    while(1){
        if(Read_Status_Display()&(3<<2))  //STA2,3
            break;
    }


    // Clear databus
    *AT91C_PIOC_CODR = bus;//clear output data register

    // Set Data to databus
    *AT91C_PIOC_SODR = (unsigned int)(Data<<2); //set output data register

    // Set dir as output (74chip)
    *AT91C_PIOC_CODR = pin50;//clear output data register

    // Enable output (74chip)
    *AT91C_PIOC_CODR = pin51;//clear output data register

    // Set databus as output
    *AT91C_PIOC_OER = bus;

    // Clear C/D signal High (0 = Data)
    *AT91C_PIOC_CODR = pin49;//clear output data register

    // Clear chip select display
    *AT91C_PIOC_CODR = pin48;//clear output data register

    // Clear write display
    *AT91C_PIOC_CODR = pin46;//clear output data register

    delay(10);

    //Set chip enable display
    *AT91C_PIOC_SODR = pin48;//set output data register

    // Set write display
    *AT91C_PIOC_SODR = pin46;//set output data register

    // Disable output (74chip)
    *AT91C_PIOC_SODR = pin51;//set output data register

    // Make databus as input
    *AT91C_PIOC_ODR = bus; //output disable register

}

void dispGraphics(void){
    // Clear reset display
    *AT91C_PIOD_CODR = pin25;

    delay(10);

    // Set reset display
    *AT91C_PIOD_SODR = pin25;

    Write_Data_2_Display(0x00);
    Write_Data_2_Display(0x00);
    Write_Command_2_Display(0x40);//Set text home address
    Write_Data_2_Display(0x00);
    Write_Data_2_Display(0x40);
    Write_Command_2_Display(0x42); //Set graphic home address
    Write_Data_2_Display(0x1e);
    Write_Data_2_Display(0x00);
    Write_Command_2_Display(0x41); // Set text area
    Write_Data_2_Display(0x1e);
    Write_Data_2_Display(0x00);
    Write_Command_2_Display(0x43); // Set graphic area
    Write_Command_2_Display(0x80); // text mode
    Write_Command_2_Display(0x94); // Text on graphic off

}

void initDisplay()
{
    *AT91C_PMC_PCER = (1<<14); //Enable D
    *AT91C_PIOD_PER = pin28; // Periphial Enable Register
    *AT91C_PIOD_OER = pin28; //Output Enable Register
    *AT91C_PIOD_PPUDR = pin28; //Disable pull-up

    *AT91C_PIOD_PER = pin27;
    *AT91C_PIOD_OER = pin27;

    *AT91C_PIOD_PER = pin25;
    *AT91C_PIOD_OER = pin25;

    //DISPLAY CHIP ENABLING
    *AT91C_PIOC_OER = pin50; // DIR
    *AT91C_PIOC_OER = pin51; // OE
    *AT91C_PIOC_OER = pin49; // C/D
    *AT91C_PIOC_OER = pin48; // CE
    *AT91C_PIOC_OER = pin47; // RD
    *AT91C_PIOC_OER = pin46; // WR
    *AT91C_PIOC_OER = pin45; // FS
    *AT91C_PIOC_OER = pin44; // RV

    *AT91C_PMC_PCER = (1<<13); // Enable C
    *AT91C_PIOC_PER = bus; //Enable all pins 34-41 and 50-51

    // Row inputs
    *AT91C_PIOC_PER = pin37;
    *AT91C_PIOC_PER = pin34;
    *AT91C_PIOC_PER = pin35;
    *AT91C_PIOC_PER = pin36;

    //Enable pull-up register for the rows
    *AT91C_PIOC_PPUER = pin37;
    *AT91C_PIOC_PPUER = pin34;
    *AT91C_PIOC_PPUER = pin35;
    *AT91C_PIOC_PPUER = pin36;

    //Enable pull-up register for the columns
    *AT91C_PIOC_PPUER = pin40;
    *AT91C_PIOC_PPUER = pin41;
    *AT91C_PIOC_PPUER = pin39;

    // Disable pull-up registers
    *AT91C_PIOC_PPUDR = (255<<12); //DISABLE PULL-UPP REGISTER FOR PINS: 14-19

}

void clearDisplay(){
  
    Write_Data_2_Display(0x0);
    Write_Data_2_Display(0x0);
    Write_Command_2_Display(0x24);
    delay(10);
  for(int i = 0; i<500; i++)
  {
    printKey(14);
  }
 }
void printKey(int key)
{
    switch (key)
    {
        case 1:
            Write_Data_2_Display(0x11); //1
            Write_Data_2_Display(0x11);
            break;
        case 2:
            Write_Data_2_Display(0x12);//2
            Write_Data_2_Display(0x12);
            break;
        case 3:
            Write_Data_2_Display(0x13);//3
            Write_Data_2_Display(0x13);
            break;
        case 4:
            Write_Data_2_Display(0x14);//4
            Write_Data_2_Display(0x14);
            break;
        case 5:
            Write_Data_2_Display(0x15);//5
            Write_Data_2_Display(0x15);
            break;
        case 6:
            Write_Data_2_Display(0x16);//6
            Write_Data_2_Display(0x16);
            break;
        case 7:
            Write_Data_2_Display(0x17);//7
            Write_Data_2_Display(0x17);
            break;
        case 8:
            Write_Data_2_Display(0x18);//8
            Write_Data_2_Display(0x18);
            break;
        case 9:
            Write_Data_2_Display(0x19);//9
            Write_Data_2_Display(0x19);
            break;
       
        case 0:
            Write_Data_2_Display(0x10);//0
            Write_Data_2_Display(0x10);
            break;
        case 11:
            Write_Data_2_Display(0xC);//,
            Write_Data_2_Display(0xC);
            break;
        case 12:
            Write_Data_2_Display(0xA);//star
            Write_Data_2_Display(0xA);
            break;
        case 13:
            Write_Data_2_Display(0x23);//C
            Write_Data_2_Display(0x23);
            break;
        case 14:
            Write_Data_2_Display(0x5F);//space
            Write_Data_2_Display(0x5F);
            break;

    }
    Write_Command_2_Display(0xC0); // Data wright and Increment ADP
}