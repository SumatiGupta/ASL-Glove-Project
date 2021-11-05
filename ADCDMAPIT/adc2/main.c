#include "main.h"

#define LED_RED (1) //on port A
#define LED_YELLOW (2) //on port A
#define	LED_GREEN	(12) //on port A
#define	LED_GREEN2 (4) //on port A
#define LED_BLUE (4) // on port D 
#define CONVERSION_LED (5) //on port A 

#define MASK(x) (1UL << (x))
#define GLOVEMASK(x) ((x) >> 16)

int shiftedpinky = 0; 
int shiftedring = 0; 
int shiftedmiddle = 0; 
int shiftedindex = 0; 
int shiftedthumb = 0; 

int main(void){
	
	// Test code to see if ADC works 
	// Enable clocks
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
	PORTA->PCR[CONVERSION_LED] &= ~PORT_PCR_MUX_MASK; 
	PORTA->PCR[CONVERSION_LED] |= PORT_PCR_MUX(1);
	
	//set LED bits as outputs 
	PTA->PDDR |= MASK(LED_RED) | MASK(LED_YELLOW) | MASK(LED_GREEN) | MASK(LED_GREEN2) | MASK(CONVERSION_LED);
	PTD->PDDR |= MASK(LED_BLUE);
	//Switch off all LEDS intially 
	PTA->PCOR = MASK(LED_RED) | MASK(LED_YELLOW)| MASK(LED_GREEN) | MASK(LED_GREEN2) | MASK(CONVERSION_LED);
	PTD->PCOR |= MASK(LED_BLUE); 
		
	adc_init(); 
	dma_init();
	pit_init(); 
	
	while (1){
		
		shiftedpinky = GLOVEMASK(GloveInputs[pinky_index]);
		shiftedring = GLOVEMASK(GloveInputs[ring_index]);
		shiftedmiddle = GLOVEMASK(GloveInputs[middle_index]);
		shiftedindex = GLOVEMASK(GloveInputs[index_index]);
		shiftedthumb = GLOVEMASK(GloveInputs[thumb_index]);
		//PTA->PSOR |= MASK(LED_RED) | MASK(LED_YELLOW) | MASK(LED_GREEN) | MASK(LED_GREEN2) | MASK(CONVERSION_LED);
		//PTD->PSOR |= MASK(LED_BLUE);
		if (shiftedpinky > 0x200){
			PTA->PSOR = MASK(LED_RED); 
		}
		else {
			PTA->PCOR = MASK(LED_RED);
		}
		
		if (shiftedring > 0x200){
			PTA->PSOR = MASK(LED_YELLOW); 
		}
		else {
			PTA->PCOR = MASK(LED_YELLOW);
		}
		
		if (shiftedmiddle > 0x200){
			PTD->PSOR = MASK(LED_BLUE); 
		}
		else {
			PTD->PCOR = MASK(LED_BLUE);
		}
		if (shiftedindex > 0x200){
			PTA->PSOR = MASK(LED_GREEN); 
		}
		else {
			PTA->PCOR = MASK(LED_GREEN);
		}
		if (shiftedthumb > 0x200){
			PTA->PSOR = MASK(LED_GREEN2); 
		}
		else {
			PTA->PCOR = MASK(LED_GREEN2);
		}
		/*if (conversion_done == 8){
			PTA->PSOR = MASK(CONVERSION_LED); 
		}
		else {
			PTA->PCOR = MASK(CONVERSION_LED);
		}*/
		
	
	}
	
}
