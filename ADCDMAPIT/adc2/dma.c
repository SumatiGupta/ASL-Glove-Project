#include "dma.h"

//variable definitions 
uint32_t  GloveInputs[8];
volatile int conversion_done = 0; 

void dma_init(void)
{
	// Enable clocks
	SIM->SCGC6 |= SIM_SCGC6_DMAMUX_MASK;
	SIM->SCGC7 |= SIM_SCGC7_DMA_MASK;
	
	// Config DMA Mux for ADC operation
	// Disable DMA Mux channel first
	DMAMUX0->CHCFG[0] = 0x00;
	
	//configure DMA_SAR to read the ADC_Rn 
	//configure DMA_DAR to write in array
	DMA0->DMA[0].SAR = DMA_SAR_SAR((uint32_t *)(&(ADC0->R[0])));
	DMA0->DMA[0].DAR = DMA_DAR_DAR((uint32_t *)GloveInputs);
	
	
	// initialize byte count - 2 bytes at a time - 16 bits. 16 bits selected since ADC in 16 bit mode  
	DMA0->DMA[0].DSR_BCR = DMA_DSR_BCR_BCR(2);
	
	//if we want to force one read at a time - this means two bytes is one read 
	DMA0->DMA[0].DCR |= DMA_DCR_CS_MASK;
	
	DMA0->DMA[0].DCR |= (DMA_DCR_EINT_MASK|		// Enable interrupt each time transfer is over
				 DMA_DCR_ERQ_MASK |		// Enable peripheral request
				 DMA_DCR_SSIZE(2) |		// Set source size to 16 bits
				 DMA_DCR_DINC_MASK|		// Set increments to destination address
				 DMA_DCR_DMOD(2)  |     // Destination address modulo of 16 bytes
				 DMA_DCR_DSIZE(2));		// Set destination size of 16 bits 
				 //set bit 24 to 0 (a requirement) 
	DMA0->DMA[0].DCR &= ~DMA_DCR_SINC_MASK; //clears source increment
	
	DMA0->DMA[0].DCR |= DMA_DCR_EADREQ_MASK; // changed in office hours 
	
	
	// Enable interrupt
	NVIC_SetPriority(DMA0_IRQn, 2);
	NVIC_ClearPendingIRQ(DMA0_IRQn);
	NVIC_EnableIRQ(DMA0_IRQn);
	
	
	// Enable DMA channel and source - ADC0 = 40 based on Table 3-20: DMA request sources
	DMAMUX0->CHCFG[0] |= DMAMUX_CHCFG_SOURCE(40); // Enable DMA channel and set ADC0 as source
	DMAMUX0->CHCFG[0] |= DMAMUX_CHCFG_ENBL_MASK;
	
	/*if ((DMA0->DMA[0].DAR >= GloveInputs) && (DMA0->DMA[0].DAR <= (GloveInputs + 5))){
			DMAMUX0->CHCFG[0] |= DMAMUX_CHCFG_ENBL_MASK;
	}
	else{
				//DMAMUX0->CHCFG[0] = 0x00;
				DMA0->DMA[0].DAR = DMA_DAR_DAR((uint32_t *)GloveInputs);
				DMAMUX0->CHCFG[0] |= DMAMUX_CHCFG_ENBL_MASK;
	}*/
	
	DMA0->DMA[0].DSR_BCR &= ~DMA_DSR_BCR_DONE_MASK;
	
	
}

void DMA0_IRQHandler(void)
{
	conversion_done += 1; 	
	
	//everytime conversion completes, clears BCR to 0, so we need to clear done flag and initialize byte count again
	DMA0->DMA[0].DSR_BCR |= DMA_DSR_BCR_DONE_MASK; // Clear Done Flag - write to it to clear it
	DMA0->DMA[0].DSR_BCR = DMA_DSR_BCR_BCR(2);  
	

	if (conversion_done == 8){
		//UART stuff 
	}

}
