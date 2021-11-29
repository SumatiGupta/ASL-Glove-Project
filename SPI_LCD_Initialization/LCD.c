#include "LCD.h"
#include <stdio.h>
#include <stdlib.h>


void Init_LCD(void){
	//code to set up pins for RS and RW 
	//Enable clock to port A,B & E

	
	//SeT PTD5 as RegisterSelect
  /*PORTD->PCR[5] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[5] |= PORT_PCR_MUX(1);
  //Set PTD0 as Enable
  PORTD->PCR[0] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[0] |= PORT_PCR_MUX(1);	
	//Set PTA13 as Read/Write 
	PORTA->PCR[13] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[13] |= PORT_PCR_MUX(1);
	
	//Set EN 
  PTD->PSOR |= MASK(EN);
 //Clear RS & RW
  PTD->PSOR |= MASK(RS);
  PTA->PSOR |= MASK(RW);	*/
	
	
	//code to intiliaze LCD according to datasheet. 
	//  Wait for power stabilization 500ms:
	Delay(500);
	
	//Function set: 00111(N)(F)(FT1)(FT0)
	//  N = lines: N=1 is 2 lines
  //  F = Font: 0 = 5x8, 1 = 5x10
  //  FT = Font Table:
  //  FT=00 is English/Japanese ~"standard" for character LCDs
  //  FT=01 is Western European I fractions, circle-c some arrows
  //  FT=10 is English/Russian
  //  FT=11 is Western European II my favorite, arrows, Greek letters
	while(!(SPI0->S & SPI_S_SPTEF_MASK))
		;  //Wait for transmit buffer empty 
	SPI0->D = 0x3B; 
	
	//Display On/Off Control RS=0,R/W=0
  // 0000 1DCB
  //  D = Display On
  //  C = Cursor On
  //  B = Cursor Blink
  while(!(SPI0->S & SPI_S_SPTEF_MASK))
		;  //Wait for transmit buffer empty 
	SPI0->D = (0x0E);
	
	
	//Graphic vs character mode setting, RS=0,R/W=0
  // 0001 GP11
  //  G = Mode: 1=graphic, 0=character
  //  C = Power: 1=0n, 0=off
	//while(!(SPI0->S & SPI_S_SPTEF_MASK))
	//	;  //Wait for transmit buffer empty 
	//SPI0->D = 0x17; 
	
	clear_display(); 
	
	//Display home
 while(!(SPI0->S & SPI_S_SPTEF_MASK))
		;  //Wait for transmit buffer empty 
	SPI0->D = (0x02);
 
 //Entry Mode Set RS=0,R/W=0
  // 0000 01IS
  //  I = Increment/or decrement
  //  S = Shift(scroll) data on line
  while(!(SPI0->S & SPI_S_SPTEF_MASK))
		;  //Wait for transmit buffer empty 
	SPI0->D = (0x06);
	
	Delay(2000);
  clear_display();
  Delay(2000);
}
	

void Delay(volatile unsigned int time_del){
	while(time_del--){
		;
	}
}

void clear_display(void)
{
  //Display Clear RS=0,R/W=0
  // 0000 0001
  while(!(SPI0->S & SPI_S_SPTEF_MASK))
		;  //Wait for transmit buffer empty 
	SPI0->D = (0x01);
  Delay(60000);
}
//
