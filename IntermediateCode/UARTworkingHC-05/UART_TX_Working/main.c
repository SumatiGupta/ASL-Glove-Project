#include "main.h" 

#define LED_RED (1) //on port A
#define LED_YELLOW (2) //on port A
#define	LED_GREEN	(12) //on port A
#define	LED_GREEN2 (4) //on port A
#define LED_BLUE (5) // on port A 
#define MASK(x) (1UL << (x))

int index = 0;

int main (void){
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTD_MASK; 
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
	
	//Select GPIO pin for 6 LED outputs
	PORTA->PCR[LED_RED] &= ~PORT_PCR_MUX_MASK; 
	PORTA->PCR[LED_RED] |= PORT_PCR_MUX(1);
	PORTA->PCR[LED_YELLOW] &= ~PORT_PCR_MUX_MASK; 
	PORTA->PCR[LED_YELLOW] |= PORT_PCR_MUX(1);
	PORTA->PCR[LED_GREEN] &= ~PORT_PCR_MUX_MASK; 
	PORTA->PCR[LED_GREEN] |= PORT_PCR_MUX(1);
	PORTA->PCR[LED_GREEN2] &= ~PORT_PCR_MUX_MASK; 
	PORTA->PCR[LED_GREEN2] |= PORT_PCR_MUX(1);
	PORTA->PCR[LED_BLUE] &= ~PORT_PCR_MUX_MASK; 
	PORTA->PCR[LED_BLUE] |= PORT_PCR_MUX(1);
	
	//set LED bits as outputs 
	PTA->PDDR |= MASK(LED_RED) | MASK(LED_YELLOW) | MASK(LED_GREEN) | MASK(LED_GREEN2);
	PTA->PDDR |= MASK(LED_BLUE);
	
	//Switch off all LEDS intially 
	PTA->PCOR = MASK(LED_RED) | MASK(LED_YELLOW)| MASK(LED_GREEN) | MASK(LED_GREEN2);
	PTA->PCOR |= MASK(LED_BLUE); 
	
	GloveInputs[pinky_index] = 0x33330000; 
	GloveInputs[ring_index] = 0x43330000; 
	GloveInputs[middle_index] = 0x53330000; 
	GloveInputs[index_index] = 0x63330000; 
	GloveInputs[thumb_index] = 0x73330000; 
	
	
	Init_UART();
	TransmitBufferCreate();
	
	
	while (1){
		
		 
		//the code below this will be in DMA, with conversion done = 8
		//dma.h should include UART.h 
		//code will in be in index based while or for loop
		
		//UART0_Transmit_OneByte();
		 	
		UART0_Transmit_DMA(index);
		index++; 
		if(index > 11) //if you change 11 to a 10, the uart might be on the cusp of sending/not sending
									 //11 gives the uart plenty of time to reset the index, etc. 
		{	
			UART0 -> C2 &= ~UART0_C2_TE_MASK;
			index=0;
		}
	} 		
}