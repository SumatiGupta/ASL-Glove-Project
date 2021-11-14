#include "main.h" 

#define LED_RED (1) //on port A
#define LED_YELLOW (2) //on port A
#define	LED_GREEN	(12) //on port A
#define	LED_GREEN2 (4) //on port A
#define LED_BLUE (4) // on port D 
#define MASK(x) (1UL << (x))

int main (void){
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTD_MASK; 
	
	//Select GPIO pin for 6 LED outputs
	PORTA->PCR[LED_RED] &= ~PORT_PCR_MUX_MASK; 
	PORTA->PCR[LED_RED] |= PORT_PCR_MUX(1);
	PORTA->PCR[LED_YELLOW] &= ~PORT_PCR_MUX_MASK; 
	PORTA->PCR[LED_YELLOW] |= PORT_PCR_MUX(1);
	PORTA->PCR[LED_GREEN] &= ~PORT_PCR_MUX_MASK; 
	PORTA->PCR[LED_GREEN] |= PORT_PCR_MUX(1);
	PORTA->PCR[LED_GREEN2] &= ~PORT_PCR_MUX_MASK; 
	PORTA->PCR[LED_GREEN2] |= PORT_PCR_MUX(1);
	PORTD->PCR[LED_BLUE] &= ~PORT_PCR_MUX_MASK; 
	PORTD->PCR[LED_BLUE] |= PORT_PCR_MUX(1);
	
	//set LED bits as outputs 
	PTA->PDDR |= MASK(LED_RED) | MASK(LED_YELLOW) | MASK(LED_GREEN) | MASK(LED_GREEN2);
	PTD->PDDR |= MASK(LED_BLUE);
	
	//Switch off all LEDS intially 
	PTA->PCOR = MASK(LED_RED) | MASK(LED_YELLOW)| MASK(LED_GREEN) | MASK(LED_GREEN2);
	PTD->PCOR |= MASK(LED_BLUE); 
	
	Init_UART();
	
	while (1){
		UART0_Receive_DMA(); 
		if (receivedbuffer[pinky_index] > 0x200){
			PTA->PSOR = MASK(LED_RED); 
		}
		else {
			PTA->PCOR = MASK(LED_RED);
		}
		
		if (receivedbuffer[ring_index] > 0x200){
			PTA->PSOR = MASK(LED_YELLOW); 
		}
		else {
			PTA->PCOR = MASK(LED_YELLOW);
		}
		
		if (receivedbuffer[middle_index] > 0x200){
			PTD->PSOR = MASK(LED_BLUE); 
		}
		else {
			PTD->PCOR = MASK(LED_BLUE);
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
	}
}