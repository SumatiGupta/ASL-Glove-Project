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
	
	Init_UART();
	
	
	
	while (1){
		GloveInputs[pinky_index] = 0x33; 
		GloveInputs[ring_index] = 0x43; 
		GloveInputs[middle_index] = 0x53; 
		GloveInputs[index_index] = 0x63; 
		GloveInputs[thumb_index] = 0x73; 	
		UART0_Transmit_DMA(index);
		UART0_Receive_DMA(index);
 
		if (receivedbuffer[pinky_index] > 0x010){
			PTA->PSOR = MASK(LED_RED); 
		}
		else {
			PTA->PCOR = MASK(LED_RED);
		}
		index++; 
		if(index > 4) index = 0;
		
		/*
		if (receivedbuffer[ring_index] > 0x200){
			PTA->PSOR = MASK(LED_YELLOW); 
		}
		else {
			PTA->PCOR = MASK(LED_YELLOW);
		}
		
		if (receivedbuffer[middle_index] > 0x200){
			PTA->PSOR = MASK(LED_BLUE); 
		}
		else {
			PTA->PCOR = MASK(LED_BLUE);
		}
		if (receivedbuffer[index_index] > 0x200){
			PTA->PSOR = MASK(LED_GREEN); 
		}
		else {
			PTA->PCOR = MASK(LED_GREEN);
		}
		if (receivedbuffer[thumb_index] > 0x200){
			PTA->PSOR = MASK(LED_GREEN2); 
		}
		else {
			PTA->PCOR = MASK(LED_GREEN2);
		}
	
	  GloveInputs[pinky_index] += 0x10; 
		GloveInputs[ring_index] += 0x10; 
		GloveInputs[middle_index] += 0x10; 
		GloveInputs[index_index] += 0x10; 
		GloveInputs[thumb_index] += 0x10;
		*/
	} 		
}