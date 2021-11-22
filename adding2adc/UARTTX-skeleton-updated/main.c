#include "main.h" 

#define LED_RED (1) //on port A
#define LED_YELLOW (2) //on port A
#define	LED_GREEN	(12) //on port A
#define	LED_GREEN2 (4) //on port A
#define LED_BLUE (4) // on port D 
#define MASK(x) (1UL << (x))

int index = 0;

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
		TransmitBufferCreate(); 	
		UART0_Transmit_DMA(index);
		index++; 
		if(index > 15) index = 0;
	}
}
//Create a function called read switches that is called in while 
//(1) in main and updates cal_val 
//have a timer that interrupts every 2 secs
//if conversion = 8 and cal_val = 1, start timer in DMA 
//cal_val = 1 means that standing position readings are being read
//after 2 secs change cal_val = 2. This indicates that the readings 
//are bent position. After 2 secs, change cal_val = 0