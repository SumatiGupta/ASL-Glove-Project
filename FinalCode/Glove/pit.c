#include "pit.h"

int counter = 0; 

void pit_init(void)
{
	// Enable PIT clock
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
	
	
	// Turn on PIT - clear MDIS bit in MCR reg
	PIT->MCR &= ~PIT_MCR_MDIS_MASK;
	//freeze timer in debug mode 
	PIT->MCR |= PIT_MCR_FRZ_MASK;
	
	// Configure PIT to produce an interrupt every 1s on Timer 0
	PIT->CHANNEL[0].LDVAL  = PIT_LDVAL_TSV(0x5DBF);	// 1ms*24Mhz - 1 = 23999 cycles or 5DBF
	// Enable interrupt and enable timer 
	PIT->CHANNEL[0].TCTRL  |= PIT_TCTRL_TIE_MASK | PIT_TCTRL_TEN_MASK; 
	//set "no chaining"
	PIT->CHANNEL[0].TCTRL &= ~PIT_TCTRL_CHN_MASK; 
	
	// Configure PIT to produce an interrupt every 2s on Timer 1
	PIT->CHANNEL[1].LDVAL  = PIT_LDVAL_TSV(0x2DC6BFF);	// 2*24Mhz - 1 = 47999999 cycles or 2DC6BFF
	// Enable interrupt and enable timer 
	PIT->CHANNEL[1].TCTRL  |= PIT_TCTRL_TIE_MASK | PIT_TCTRL_TEN_MASK; 
	//set "no chaining"
	PIT->CHANNEL[1].TCTRL &= ~PIT_TCTRL_CHN_MASK;


	// Enable interrupt
	NVIC_SetPriority(PIT_IRQn, 0);
	NVIC_ClearPendingIRQ(PIT_IRQn);
	NVIC_EnableIRQ(PIT_IRQn);
	__enable_irq(); 
	
}

void PIT_IRQHandler(void)
{	
	//Can chose to check channel int flag 
	if(PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK){
		adc_read(counter);
		counter += 1; 
		if (counter == 8){
			counter = 0;
		}
		//Clear interrupt request flag for channel by writing one to it - slide 12 of chapter 7
		PIT->CHANNEL[0].TFLG |= PIT_TFLG_TIF_MASK;
	}
	
	else if(PIT->CHANNEL[1].TFLG & PIT_TFLG_TIF_MASK)
	{
		if (cal_val == 2)
			cal_val = 0;
		else if (cal_val == 1)
			cal_val++; 
		
	  //Clear interrupt request flag for channel by writing one to it - slide 12 of chapter 7
		PIT->CHANNEL[1].TFLG |= PIT_TFLG_TIF_MASK;
	}
	 
}
