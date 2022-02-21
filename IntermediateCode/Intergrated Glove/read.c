#include"read.h"

uint8_t cal_val;
//cal_val = 0 indicates performance mode 
//cal_val = 1 indicates calibration of standing up 
//cal_val = 2 indicates calibration of bent position

void Init_CAL_GPIO(void){
	
	// Test code to see if ADC works 
	// Enable clocks
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK ; 
	
	
	//Select GPIO pin for 6 LED outputs
	PORTB->PCR[CAL_LED] &= ~PORT_PCR_MUX_MASK; 
	PORTB->PCR[CAL_LED] |= PORT_PCR_MUX(1);
	PORTA->PCR[CAL_SW] &= ~PORT_PCR_MUX_MASK; 
	PORTA->PCR[CAL_SW] |= PORT_PCR_MUX(1);
	
	//set LED bits as outputs 
	PTB->PDDR |= MASK(CAL_LED);
	
	//Set sw as input
	PTA->PDDR &= ~MASK(CAL_SW);
	
	//Switch off all LEDS intially 
	PTB->PSOR = MASK(CAL_LED); 
	
}

void Read_Switches(void){
	if(PTA->PDIR & MASK(CAL_SW))
	{
		//restart timer 
		PIT->MCR |= PIT_MCR_MDIS_MASK;
		PIT->MCR &= ~PIT_MCR_MDIS_MASK;
		cal_val = 1; 
	}
}

void Light_LED(void){
	if(cal_val > 0)
	{
		PTB->PCOR = MASK(CAL_LED); 
	}
	else
	{
		PTB->PSOR = MASK(CAL_LED); 
	}
}

