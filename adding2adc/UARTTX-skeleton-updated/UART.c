#include "UART.h"

int shiftedpinky = 0; 
int shiftedring = 0; 
int shiftedmiddle = 0; 
int shiftedindex = 0; 
int shiftedthumb = 0; \
int pressure1 = 0;
int pressure2 = 0;
uint8_t cal_val = 0;
uint8_t transmitbuffer[15];
uint32_t  GloveInputs[8];
unsigned char Transmit_Done = 0;

void Init_UART(void)
{
	// Let's use UART0 and PTB 
	// Enable clock gating for UART0 and Port B
	
	uint16_t sbr; 
	
	//We use transmit data to LCD screen and maybe eventually receive (for error check) 
	//Since the UART can only take 8 bits, and we have 5 important 16 bit values that 
	//need to be sent, we would have to fill D reg with each half of an array element
	//making it a total of 5*2 times D needs to be updated. 
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK; 										
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;											
	
	// Make sure transmitter and receiver are disabled before configuring
	UART0->C2 &= ~UART0_C2_TE_MASK & ~UART0_C2_RE_MASK; 		
	
	// UART 1 has to use Bus Clock, clock can't be configured
	// according to table 5-2 (page 122) of reference manual 
	// and absence of macors 
	// but UART0 clock needs to be set - to 48 MHz clock in this case
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);
	//SIM->SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;
	
	// Set pins to UART0 Rx and Tx
	PORTE->PCR[TX_PIN] |= PORT_PCR_MUX(4); // Tx
	//PORTE->PCR[RX_PIN] |= PORT_PCR_MUX(4); // Rx
	
	// Set baud rate and oversampling ratio
	sbr = (uint16_t)((CLOCK)/(BAUD_RATE * OVERSAMPLE_RATE)); 			
	UART0->BDH &= ~UART0_BDH_SBR_MASK;
	UART0->BDH |= UART0_BDH_SBR(sbr>>8);
	UART0->BDL = UART0_BDL_SBR(sbr);
	UART0->C4 |= UART0_C4_OSR(OVERSAMPLE_RATE-1); //UARTLP_C4_OSR(x) is same as UART0_C4_OSR(x) and since C4 reg all UART is same, it should work 	
	
	//LIN break is high whenever a break is detected. 
	//I believe this occurs when there is empty space in a transmission or recieve line 
	//this will set LBKDIF bit in UARTx_S2 as a high. 
	//setting LBKDIE bit in BDH reg will cause an interrupt if LBKDIF is high
	UART0->BDH |= UART0_BDH_LBKDIE(0);  // for now let's disable it 
	UART0->BDH |= UART0_BDH_RXEDGIE(0); // Disable interrupts for RX active edge
	UART0->BDH |= UART0_BDH_SBNS(0);   // one stop-bit - only two options 1 or 2
					  // Its logic level is the same as the signal's idle state, i.e., logic high
	UART0->C1 |= UART0_C1_M(0); //Data bit mode - 8 bits - In 8-bit data mode, the shift register holds a
					 //start bit, eight data bits, and a stop bit.
  UART0->C1 |= UART0_C1_LOOPS(1); //0: Normal operation - RxD and TxD use separate pins.
	UART0->C1 |= UART0_C1_PE(0);	//Don't use parity bit
						//We can use parity bit but will need to change bit mode to 9 - The 9-bit data mode is typically used with parity to allow eight bits of data plus the parityin the ninth bit
						//When parity is enabled, the bit immediately before the stop bit is treated as the parity bit
						// a little confused about D being only 8 bits long
						// will also need to set PT bit in C1 to parity odd or even and enable interrupt for parity (PEIE) in C3 
																
	// Don't enable interrupts for errors															
	UART0->C3 = UART0_C3_ORIE(0)| UART0_C3_NEIE(0) | UART0_C3_FEIE(0) | UART0_C3_PEIE(0);															

	// Clear error flags
	UART0->S1 |= UART0_S1_OR(1) | UART0_S1_NF(1) | UART0_S1_FE(1) | UART0_S1_PF(1); //To clear these flags write logic one to them 

 	//UART0->S2 = UART0_S2_MSBF(0) | UART0_S2_RXINV(0); //do not invert recieved data
	UART0 -> C3 = UART0_C3_TXINV(0); //Send LSB first, do not invert transmitted data
	
	
	//POSSIBLE NEED IN CASE OF RECEIVER 
	//If Receive Data Register Full Flag (RDRF bit) in UARTx_S1 is high and 
	//setting RIE bit in C2 reg will cause an interrupt - clear flag intially at end of init (based on textbook)
	//UART0->C2 |= UART_C2_RIE_MASK;
	
	UART0->C2 |= UART0_C2_TE(1); // Enable UART transmitter - will need if we use transmitter
	//UART0->C2 |= UART0_C2_RE(1); // Enable UART receiver - will need 
}

void UART0_Transmit_DMA(int index) {
	
		while (!(UART0->S1 & UART0_S1_TDRE_MASK))
			;
		UART0->D = (uint8_t)(transmitbuffer[index]);
				
		if (!(UART0->S1 & UART0_S1_TDRE_MASK))
			Transmit_Done++;
	
}

void TransmitBufferCreate(){
	
	shiftedpinky = GLOVEMASK(GloveInputs[pinky_index]);
	shiftedring = GLOVEMASK(GloveInputs[ring_index]);
	shiftedmiddle = GLOVEMASK(GloveInputs[middle_index]);
	shiftedindex = GLOVEMASK(GloveInputs[index_index]);
	shiftedthumb = GLOVEMASK(GloveInputs[thumb_index]);
	pressure1 = GLOVEMASK(GloveInputs[5]);
	pressure2 = GLOVEMASK(GloveInputs[6]);
	
	transmitbuffer[0] = LSB(shiftedpinky);
	transmitbuffer[1] = MSB(shiftedpinky);
	transmitbuffer[2] = LSB(shiftedring);
	transmitbuffer[3] = MSB(shiftedring);
	transmitbuffer[4] = LSB(shiftedmiddle);
	transmitbuffer[5] = MSB(shiftedmiddle);
	transmitbuffer[6] = LSB(shiftedindex);
	transmitbuffer[7] = MSB(shiftedindex);
	transmitbuffer[8] = LSB(shiftedthumb);
	transmitbuffer[9] = MSB(shiftedthumb);
	transmitbuffer[10] = LSB(pressure1);
	transmitbuffer[11] = MSB(pressure1);
	transmitbuffer[12] = LSB(pressure2);
	transmitbuffer[13] = MSB(pressure2);
	transmitbuffer[14] = cal_val; 
}
