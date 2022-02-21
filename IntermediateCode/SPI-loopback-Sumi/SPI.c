#include "SPI.h"

void Init_SPI(void){
	//Enable clock to SPI0
	SIM->SCGC4 |= SIM_SCGC4_SPI0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
	
	//Disable SPI0 to allow configuration
	SPI0->C1 &= ~SPI_C1_SPE_MASK;
	
	//set PTC5 as SPI0_SCK --ALT2
	PORTC->PCR[5] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[5] |= PORT_PCR_MUX(2);
	//set PTC6 as SPI0_MOSI --ALT5
	PORTC->PCR[6] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[6] |= PORT_PCR_MUX(2);
	//set PTC7 as SPI0_MISO --ALT5
	PORTC->PCR[7] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[7] |= PORT_PCR_MUX(2);
	//set PTC4 as SPI0_PCS0 --ALT2
	PORTC->PCR[4] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[4] |= PORT_PCR_MUX(2);
	
	//Set SPI0 to Master and SS pin to auto Slave select
	SPI0->C1 = SPI_C1_MSTR_MASK | SPI_C1_SSOE_MASK;
	SPI0->C2 = SPI_C2_MODFEN_MASK;
	//Select active high clock, first edge sample
	SPI0->C1 &= ~SPI_C1_CPHA_MASK;
	SPI0->C1 &= ~SPI_C1_CPOL_MASK;
	//Baudrate = Busclock / ((SPPR+1)*2^(SPR+1))
	SPI0->BR = SPI_BR_SPPR(2) | SPI_BR_SPR(1);
	
	//enable SPI0
	SPI0->C1 |= SPI_C1_SPE_MASK;
}

uint8_t test_SPIsend(uint8_t d_out)
{
	while(!(SPI0->S & SPI_S_SPTEF_MASK))
		;  //Wait for transmit buffer empty 
	SPI0->D = d_out;

	while (!(SPI0->S & SPI_S_SPRF_MASK))
		; // wait for receive buffer full
	return SPI0->D;
}

void Test_SPI_Loopback(void) {
	uint8_t out='A', in;
	while (1) {
		in = test_SPIsend(out);
		if (in != out) {
			// Red: error, data doesn't match
			Control_RGY_LEDs(1, 0, 0);
		} else {
			// Green: data matches
			Control_RGY_LEDs(0, 1, 0);
		}
		out++;
		if (out > 'Z') 
			out = 'A';
	}
}

void Control_RGY_LEDs(int r_on, int g_on, int y_on) {
	if (r_on)
		PTA->PSOR = MASK(LED_RED);
	else
		PTA->PCOR = MASK(LED_RED);
	if (g_on)
		PTA->PSOR = MASK(LED_GREEN);
	else
		PTA->PCOR = MASK(LED_GREEN);
	if (y_on)
		PTA->PSOR = MASK(LED_YELLOW);
	else
		PTA->PCOR = MASK(LED_YELLOW);
}

