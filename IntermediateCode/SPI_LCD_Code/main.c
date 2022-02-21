#include "main.h"

#define SW_1 (9) //on port C

int main(void){
	//Enable clock to PORT C
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
	
	PORTC->PCR[SW_1] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[SW_1] |= PORT_PCR_MUX(1);
	//clear the bit to input
	PTB->PDDR &= ~MASK(SW_1);
	while(1){
		//Turn On Display
	if(PTB->PDIR & MASK(SW_1)){
			PTA->PSOR = MASK(DB3) | MASK(DB2) | MASK(DB1) | MASK(DB0);
		  
	}
 }
}
	
	
	
	
	
	
