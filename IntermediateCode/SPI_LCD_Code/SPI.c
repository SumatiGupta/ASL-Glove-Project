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
