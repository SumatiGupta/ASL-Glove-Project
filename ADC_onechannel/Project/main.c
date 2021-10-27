#include "main.h"

#define LED_RED (1) //on port A
#define LED_YELLOW (2) //on port A

#define MASK(x) (1UL << (x))

int main(void){
	
	// Test code to see if ADC works 
	// Enable clocks
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK; 
	
	
	//Select analog pin for 5 fingers
	PORTA->PCR[LED_RED] &= ~PORT_PCR_MUX_MASK; 
	PORTA->PCR[LED_RED] |= PORT_PCR_MUX(1);
	PORTA->PCR[LED_YELLOW] &= ~PORT_PCR_MUX_MASK; 
	PORTA->PCR[LED_YELLOW] |= PORT_PCR_MUX(1);
	
	//set LED bits as outputs 
	PTA->PDDR |= MASK(LED_RED) | MASK(LED_YELLOW);
	//Switch off all LEDS intially 
	PTA->PCOR = MASK(LED_RED) | MASK(LED_YELLOW);
	
		
	adc_init(); 
	
	while (1){
		adc_read(); 
	  //pinky 
		if ((float)(ADC0->R[0]) > 1000){
			PTA->PSOR = MASK(LED_RED); 
		}
		else {
			PTA->PCOR = MASK(LED_RED);
		}
	
		//ring 
		//if (ADC0->R[1] > 50){
		//	PTA->PSOR = MASK(LED_YELLOW); 
		//}
	
	}
	
}
