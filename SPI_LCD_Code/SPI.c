#include "SPI.h"

void Init_SPI(void){
	//Enable clock to SPI1
	SIM->SCGC4 |= SIM_SCGC4_SPI1_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
	
	//Disable SPI1 to allow configuration
	SPI1->C1 &= ~SPI_C1_SPE_MASK;
	
	//set PTE2 as SPI1_SCK --ALT2
	PORTE->PCR[2] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[2] |= PORT_PCR_MUX(2);
	//set PTE3 as SPI1_MOSI --ALT5
	PORTE->PCR[3] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[3] |= PORT_PCR_MUX(5);
	//set PTE1 as SPI1_MISO --ALT5
	PORTE->PCR[1] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[1] |= PORT_PCR_MUX(5);
	//set PTE4 as SPI1_PCS0 --ALT2
	PORTE->PCR[4] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[4] |= PORT_PCR_MUX(2);
	
	//Set SPI1 to Master and SS pin to auto Slave select
	SPI1->C1 = SPI_C1_MSTR_MASK | SPI_C1_SSOE_MASK;
	SPI1->C2 = SPI_C2_MODFEN_MASK;
	//Select active high clock, first edge sample
	SPI1->C1 &= ~SPI_C1_CPHA_MASK;
	SPI1->C1 &= ~SPI_C1_CPOL_MASK;
	//Baudrate = Busclock / ((SPPR+1)*2^(SPR+1))
	SPI1->BR = SPI_BR_SPPR(2) | SPI_BR_SPR(1);
	
	//enable SPI1
	SPI1->C1 |= SPI_C1_SPE_MASK;
}

