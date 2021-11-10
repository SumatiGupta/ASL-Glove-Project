#include "LCD.h"


void Init_LCD(void){
	//Enable clock to port A,B & E
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTA_MASK;
	
	//SeT PTA1 as RegisterSelect
  PORTA->PCR[1] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[1] |= PORT_PCR_MUX(1);
  //Set PTA2 as Enable
  PORTA->PCR[2] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[2] |= PORT_PCR_MUX(1);	
	//Set PTB1 as Databus 0
	PORTB->PCR[1] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[1] |= PORT_PCR_MUX(1);
	//Set PTB3 as Databus 1
	PORTB->PCR[3] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[3] |= PORT_PCR_MUX(1);
	//Set PTB0 as Databus 2
	PORTB->PCR[0] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[0] |= PORT_PCR_MUX(1);
	//Set PTB2 as Databus 3
	PORTB->PCR[2] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[2] |= PORT_PCR_MUX(1);
	//Set PTA13 as Read/Write 
	PORTB->PCR[13] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[13] |= PORT_PCR_MUX(1);
	
	//Set LED bits to outputs
	PTA->PDDR |= MASK(DB0) | MASK(DB1) | MASK(DB2) | MASK(DB3);
	
	//Set EN 
  PTA->PSOR = MASK(EN);
 //Clear RS & RW
  PTB->PCOR = MASK(RS);
  PTB->PCOR = MASK(RW);	
	
}

