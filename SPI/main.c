#include "main.h"

#define SW_1 (9) //on port C

#define LED_RED (1) //on port A
#define LED_YELLOW (2) //on port A
#define	LED_GREEN	(12) //on port A

int main(void){
	//Enable clock to PORT C
	/*SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
	
	PORTC->PCR[SW_1] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[SW_1] |= PORT_PCR_MUX(1);
	//clear the bit to input
	PTB->PDDR &= ~MASK(SW_1);
	while(1){
		//Turn On Display
	if(PTB->PDIR & MASK(SW_1)){
			PTA->PSOR = MASK(DB3) | MASK(DB2) | MASK(DB1) | MASK(DB0);	  
	}
 }*/ 
	
	//Make sure to call LCD Init and SPI Init function.
	//Make sure to set the SCK, MOSI and MISO and SS pins to the 
	//respective pins on LCD before trying to turn it on. 

	// Enable clocks
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK; 
	
	
	//Select GPIO pin for 3 LED outputs
	PORTA->PCR[LED_RED] &= ~PORT_PCR_MUX_MASK; 
	PORTA->PCR[LED_RED] |= PORT_PCR_MUX(1);
	PORTA->PCR[LED_YELLOW] &= ~PORT_PCR_MUX_MASK; 
	PORTA->PCR[LED_YELLOW] |= PORT_PCR_MUX(1);
	PORTA->PCR[LED_GREEN] &= ~PORT_PCR_MUX_MASK; 
	PORTA->PCR[LED_GREEN] |= PORT_PCR_MUX(1);
	
	//set LED bits as outputs 
	PTA->PDDR |= MASK(LED_RED) | MASK(LED_YELLOW) | MASK(LED_GREEN);
	
	//Switch off all LEDS intially 
	PTA->PCOR = MASK(LED_RED) | MASK(LED_YELLOW)| MASK(LED_GREEN);
	
	Init_SPI(); 
	Test_SPI_Loopback();
	
	
}
