#include "main.h"

#define LED_RED (1) //on port A
#define LED_YELLOW (2) //on port A
#define	LED_GREEN	(12) //on port A
#define	LED_GREEN2 (4) //on port A
#define LED_BLUE (4) // on port D 
#define CONVERSION_LED (5) //on port A 

#define MASK(x) (1UL << (x))
#define GLOVEMASK(x) ((x) >> 16)

int index = 0; 

int main(void){
	
	adc_init(); 
	Init_UART();
	dma_init();
	pit_init(); 
	Init_CAL_GPIO();
	while(1)
	{
		Read_Switches(); 
		Light_LED();
		if(while_flag == 1)
		{
			TransmitBufferCreate();
			while(while_flag)
			{					 
					UART0->C2 |= UART0_C2_TE_MASK;
					if (index < 11)
					{
						UART0_Transmit_DMA(index);
					}
					index++;
					if(index > 11) //if you change 11 to a 10, the uart might be on the cusp of sending/not sending
											 //11 gives the uart plenty of time to reset the index, etc. 
					{	
						UART0->C2 &= ~UART0_C2_TE_MASK; //disable interrupt once all values are transferred
						index = 0;
						while_flag = 0; //stops while loop
						conversion_done = 0; 
					}
		 }
		}
	}
	
}
