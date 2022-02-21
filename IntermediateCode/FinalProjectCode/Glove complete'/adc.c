#include "adc.h"

int CH_idx[8]; 

void adc_init(void)
{
	// Enable clocks
	SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;	// ADC0 clock
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;	// PortB clock
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;	// Port E clock
	

	// Configure ADC
	ADC0->CFG1 = 0; // Reset register
	ADC0->CFG1 |= (ADC_CFG1_MODE(3)  |  	// 16 bits mode
				  ADC_CFG1_ADICLK(0)|	// Input Bus Clock (20-25 MHz out of reset (FEI mode))
				  ADC_CFG1_ADIV(0)) | 	// Clock divide by 1 
					ADC_CFG1_ADLPC_MASK //low power configuration
					; // short sample time
	
	//Select analog pin for 5 fingers
	PORTB->PCR[ADCpinky] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[ADCpinky] |= PORT_PCR_MUX(0);
	PORTB->PCR[ADCring] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[ADCring] |= PORT_PCR_MUX(0);
	PORTB->PCR[ADCmiddle] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[ADCmiddle] |= PORT_PCR_MUX(0);
	PORTB->PCR[ADCindex] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[ADCindex] |= PORT_PCR_MUX(0);
	PORTE->PCR[ADCthumb] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[ADCthumb] |= PORT_PCR_MUX(0);
	
	ADC0->SC2 |= ADC_SC2_DMAEN_MASK;    // DMA Enable
	
	ADC0_SC3 = 0; // Reset SC3
	
	ADC0_SC1A |= ADC_SC1_ADCH(31); // Disable module initially. Change the channel value once we start using it. 
	
	CH_idx[0] = ADCpinky_CH;
	CH_idx[1] = ADCring_CH;
	CH_idx[2] = ADCmiddle_CH;
	CH_idx[3] = ADCindex_CH;
	CH_idx[4] = ADCthumb_CH;
	CH_idx[5] = 0x10;
	CH_idx[6] = 0x10;
	CH_idx[7] = 0x10;
	
}

//Refer to page 494 to 495 for instructions on calibration procedure

int internal_calibration_adc(void)
{
	uint16_t cal_var = 0; // calibration variable 
	
	ADC0_CFG1 |= (ADC_CFG1_MODE(3)  |  	// 16 bit mode
				ADC_CFG1_ADICLK(1)|	// Input Bus Clock divided by 2 (20-25 MHz out of reset (FEI mode) / 2)
				ADC_CFG1_ADIV(2)) ;	// Clock divide by 4 (2.5-3 MHz)

	ADC0_SC3 |= ADC_SC3_AVGE_MASK |		// Enable HW average
				ADC_SC3_AVGS(3)   |		// Set HW average of 32 samples
				ADC_SC3_CAL_MASK;		// Start calibration process
	
	while(ADC0_SC3 & ADC_SC3_CAL_MASK); // Wait for calibration to end
	
	if(ADC0_SC3 & ADC_SC3_CALF_MASK)	// Check for successful calibration
		return 1; 
	
	//Do the following procedure for both plus and minus sides 
	cal_var += ADC0_CLPS + ADC0_CLP4 + ADC0_CLP3 +
			 ADC0_CLP2 + ADC0_CLP1 + ADC0_CLP0;
	cal_var = cal_var >> 2;
	cal_var |= ADC_set_MSB; 	// Set MSB 
	ADC0_PG = cal_var;
	cal_var = 0;
	cal_var += ADC0_CLMS + ADC0_CLM4 + ADC0_CLM3 +
			 ADC0_CLM2 + ADC0_CLM1 + ADC0_CLM0;
	cal_var = cal_var >> 2;
	cal_var |= ADC_set_MSB;	// Set MSB
	ADC0_MG = cal_var;
	
	return 0;
}

void adc_read(int i){
	 
	//clear coco to start conversion on for all channels - bits 4 - 0 in ADCx_SC1n selects the input channel 
	// call the calibration function before each conversion

	internal_calibration_adc(); 
  ADC0->SC1[0] = CH_idx[i]; 
	ADC0->SC1[0] &= ~ADC_SC1_COCO_MASK;
	
}
