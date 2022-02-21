#include <MKL25Z4.h>
#define MASK(x) (1UL<<(x))
#define RED_LED (1) //on port A
#define GREEN_LED (2) //on port A
#define BLUE_LED (4) //on port A

void Init_SPI(void);
void control_RGB_LEDs(int r_on, int g_on, int b_on);
void Test_SPI_Loopback(void);
uint8_t Test_SPIsend(uint8_t d_out);
void Init_GPIO(void);



 void Init_GPIO(void){
	
//Enable Clock to port A and B
SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK);
	
	PORTA->PCR[RED_LED] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[RED_LED] |= PORT_PCR_MUX(1);
	PORTA->PCR[BLUE_LED] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[BLUE_LED] |= PORT_PCR_MUX(1);
	PORTA->PCR[GREEN_LED] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[GREEN_LED] |= PORT_PCR_MUX(1);

	//Set LED bits to outputs
	PTA->PDDR |= MASK(RED_LED) | MASK(BLUE_LED) | MASK(GREEN_LED);
	//Turn off all LED's off initially
	PTA->PCOR=MASK(RED_LED);
	PTA->PCOR=MASK(BLUE_LED);
	PTA->PCOR=MASK(GREEN_LED);
}	

void Init_SPI(void){
	//Enable clock to SPI1
	SIM->SCGC4 |= SIM_SCGC4_SPI1_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
	
	//Disable SPI1 to allow configuration
	SPI1->C1 &= ~SPI_C1_SPE_MASK;
	
	//set PTE2 as SPI1_SCK --ALT2
	PORTE->PCR[2] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[2] |= PORT_PCR_MUX(2);
	//set PTE3 as SPI1_MOSI --ALT2
	PORTE->PCR[3] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[3] |= PORT_PCR_MUX(2);
	//set PTE1 as SPI1_MISO --ALT2
	PORTE->PCR[1] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[1] |= PORT_PCR_MUX(2);
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

uint8_t Test_SPIsend(uint8_t d_out){
	while(!(SPI0->S & SPI_S_SPTEF_MASK))
		;
	//control_RGB_LEDs(1,0,0);
	SPI1->D = d_out;
	while(!(SPI1->S & SPI_S_SPRF_MASK))
	//control_RGB_LEDs(0,1,0);
		;
	return SPI1->D;
}
void control_RGB_LEDs(int r_on, int g_on, int b_on){
	if (r_on)
		PTA->PSOR = MASK(RED_LED);
	else 
		PTA->PCOR = MASK(RED_LED);
	if (g_on)
		PTA->PSOR = MASK(GREEN_LED);
	else 
		PTA->PCOR = MASK(GREEN_LED);
	if (b_on)
		PTA->PSOR = MASK(BLUE_LED);
	else 
		PTA->PCOR = MASK(BLUE_LED);
}
void Test_SPI_Loopback(void)
	{
	uint8_t out='A', in;
	while(1){
		in=Test_SPIsend(out);
		if(in != out) {
			control_RGB_LEDs(1,0,0);
		}
		else{
			control_RGB_LEDs(0,1,1);
		}
		out++;
		if(out > 'z')
			out = 'A';
		}
}
	
int main(void){
	Init_SPI();
	Init_GPIO();
	Test_SPI_Loopback();
}


