#include "UART.h"

uint16_t receivedbuffer[5]; 
uint32_t  GloveInputs[8];

unsigned char Transmit_Done = 0;
unsigned char Receive_Done = 0; 

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
	PORTE->PCR[RX_PIN] |= PORT_PCR_MUX(4); // Rx
	
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

 	UART0->S2 = UART0_S2_MSBF(0) | UART0_S2_RXINV(0); //do not invert recieved data
	UART0 -> C3 = UART0_C3_TXINV(0); //Send LSB first, do not invert transmitted data
	
	
	//POSSIBLE NEED IN CASE OF RECEIVER 
	//If Receive Data Register Full Flag (RDRF bit) in UARTx_S1 is high and 
	//setting RIE bit in C2 reg will cause an interrupt - clear flag intially at end of init (based on textbook)
	//UART0->C2 |= UART_C2_RIE_MASK;
	
	UART0->C2 |= UART0_C2_TE(1); // Enable UART transmitter - will need if we use transmitter
	UART0->C2 |= UART0_C2_RE(1); // Enable UART receiver - will need if we use receiver
	
}


void UART0_Transmit_DMA(int index) {
	
	
	
		while (!(UART0->S1 & UART0_S1_TDRE_MASK))
			;
		UART0->D = (uint8_t)(GloveInputs[index]);
		
		
/*		
		while (!(UART0->S1 & UART0_S1_TDRE_MASK))
			;
		UART0->D = 0x24;*/
		
		if (!(UART0->S1 & UART0_S1_TDRE_MASK))
			Transmit_Done = 1;
	
		/*// transmit pinky data
		while (!(UART0->S1 & UART0_S1_TDRE_MASK))
			;
		UART0->D = LSB(GloveInputs[pinky_index]);
		while (!(UART0->S1 & UART0_S1_TDRE_MASK))
			;
		UART0->D = MSB(GloveInputs[pinky_index]);
		
		// transmit ring data
		while (!(UART0->S1 & UART0_S1_TDRE_MASK))
			;
		UART0->D = LSB(GloveInputs[ring_index]);
		while (!(UART0->S1 & UART0_S1_TDRE_MASK))
			;
		UART0->D = MSB(GloveInputs[ring_index]);
		
		// transmit middle data
		while (!(UART0->S1 & UART0_S1_TDRE_MASK))
			;
		UART0->D = LSB(GloveInputs[middle_index]);
		while (!(UART0->S1 & UART0_S1_TDRE_MASK))
			;
		UART0->D = MSB(GloveInputs[middle_index]);
		
		// transmit index data
		while (!(UART0->S1 & UART0_S1_TDRE_MASK))
			;
		UART0->D = LSB(GloveInputs[index_index]);
		while (!(UART0->S1 & UART0_S1_TDRE_MASK))
			;
		UART0->D = MSB(GloveInputs[index_index]);
		
		// transmit thumb data
		while (!(UART0->S1 & UART0_S1_TDRE_MASK))
			;
		UART0->D = LSB(GloveInputs[thumb_index]);
		while (!(UART0->S1 & UART0_S1_TDRE_MASK))
			;
		UART0->D = MSB(GloveInputs[thumb_index]);*/
 
}

void UART0_Receive_DMA(int index) {
	
		uint8_t LSB;
		uint8_t MSB;

		
		while (!(UART0->S1 & UART0_S1_RDRF_MASK))
			;
		receivedbuffer[index] = UART0 -> D;
		
		
		if(UART0 -> S1 &= UART0_S1_RDRF_MASK)
			Receive_Done = UART0->D;
		/*
		while (!(UART0->S1 & UART0_S1_RDRF_MASK))
			;
		UART0->D = MSB;
		receivedbuffer[pinky_index] = COMBINE(MSB, LSB);*/
		
		//receive ring data
		/*while (!(UART0->S1 & UART0_S1_RDRF_MASK))
			;
		UART0->D = MSB;
		while (!(UART0->S1 & UART0_S1_RDRF_MASK))
			;
		UART0->D = LSB;
		receivedbuffer[ring_index] = COMBINE(MSB, LSB);
		
		//receive middle data
		while (!(UART0->S1 & UART0_S1_RDRF_MASK))
			;
		UART0->D = MSB;
		while (!(UART0->S1 & UART0_S1_RDRF_MASK))
			;
		UART0->D = LSB;
		receivedbuffer[middle_index] = COMBINE(MSB, LSB);
		
		//receive index data
		while (!(UART0->S1 & UART0_S1_RDRF_MASK))
			;
		UART0->D = MSB;
		while (!(UART0->S1 & UART0_S1_RDRF_MASK))
			;
		UART0->D = LSB;
		receivedbuffer[index_index] = COMBINE(MSB, LSB);
		
		//receive thumb data
		while (!(UART0->S1 & UART0_S1_RDRF_MASK))
			;
		UART0->D = MSB;
		while (!(UART0->S1 & UART0_S1_RDRF_MASK))
			;
		UART0->D = LSB;
		receivedbuffer[thumb_index] = COMBINE(MSB, LSB);*/
 
}
