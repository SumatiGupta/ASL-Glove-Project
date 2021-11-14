#include "UART.h"

uint16_t receivedbuffer[5]; 

void Init_UART(void)
{
	// Let's use UART1 and PTE since UART1 uses PTA pins that already used for ADC
	// Enable clock gating for UART1 and Port E
	
	uint16_t sbr; 
	
	//We use transmit data to LCD screen and maybe eventually receive (for error check) 
	//Since the UART can only take 8 bits, and we have 5 important 16 bit values that 
	//need to be sent, we would have to fill D reg with each half of an array element
	//making it a total of 5*2 times D needs to be updated. 
	SIM->SCGC4 |= SIM_SCGC4_UART1_MASK; 										
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;											
	
	// Make sure transmitter and receiver are disabled before configuring
	UART1->C2 &= ~UART_C2_TE_MASK & ~UART_C2_RE_MASK; 		
	
	// UART 1 has to use Bus Clock, clock can't be configured
	// according to table 5-2 (page 122) of reference manual 
	// and absence of macors 
	
	// Set pins to UART1 Rx and Tx
	PORTE->PCR[TX_PIN] |= PORT_PCR_MUX(3); // Tx
	//PORTE->PCR[RX_PIN] |= PORT_PCR_MUX(3); // Rx
	
	// Set baud rate and oversampling ratio
	sbr = (uint16_t)((CLOCK)/(BAUD_RATE * OVERSAMPLE_RATE)); 			
	UART1->BDH &= ~UART_BDH_SBR_MASK;
	UART1->BDH |= UART_BDH_SBR(sbr>>8);
	UART1->BDL = UART_BDL_SBR(sbr);
	UART1->C4 |= UARTLP_C4_OSR(OVERSAMPLE_RATE-1); //UARTLP_C4_OSR(x) is same as UART1_C4_OSR(x) and since C4 reg all UART is same, it should work 	
	
	//LIN break is high whenever a break is detected. 
	//I believe this occurs when there is empty space in a transmission or recieve line 
	//this will set LBKDIF bit in UARTx_S2 as a high. 
	//setting LBKDIE bit in BDH reg will cause an interrupt if LBKDIF is high
	UART1->BDH |= UART_BDH_LBKDIE(0);  // for now let's disable it 
	UART1->BDH |= UART_BDH_RXEDGIE(0); // Disable interrupts for RX active edge
	UART1->BDH |= UART_BDH_SBNS(0);   // one stop-bit - only two options 1 or 2
					  // Its logic level is the same as the signal's idle state, i.e., logic high
	UART1->C1 |= UART_C1_M(0); //Data bit mode - 8 bits - In 8-bit data mode, the shift register holds a
					 //start bit, eight data bits, and a stop bit.
  UART1->C1 |= UART_C1_LOOPS(0); //0: Normal operation - RxD and TxD use separate pins.
	UART1->C1	|= UART1_C1_PE(0);	//Don't use parity bit
						//We can use parity bit but will need to change bit mode to 9 - The 9-bit data mode is typically used with parity to allow eight bits of data plus the parityin the ninth bit
						//When parity is enabled, the bit immediately before the stop bit is treated as the parity bit
						// a little confused about D being only 8 bits long
						// will also need to set PT bit in C1 to parity odd or even and enable interrupt for parity (PEIE) in C3 
																
	// Don't enable interrupts for errors															
	UART1->C3 = UART_C3_ORIE(0)| UART_C3_NEIE(0) | UART_C3_FEIE(0) | UART_C3_PEIE(0);															

	// Clear error flags
	UART1->S1 |= UART_S1_OR_MASK | UART_S1_NF_MASK | UART_S1_FE_MASK | UART_S1_PF_MASK; //To clear these flags write logic one to them 

  UART1->S2 = UART_S2_MSBF(0) | UART_S2_RXINV(0); //Send LSB first, do not invert received data
	
	//POSIIBLE NEED IN CASE OF RECEIVER 
	//If Receive Data Register Full Flag (RDRF bit) in UARTx_S1 is high and 
	//setting RIE bit in C2 reg will cause an interrupt - clear flag intially at end of init (based on textbook)
	//UART1->C2 |= UART_C2_RIE_MASK;
	
	UART1->C2 |= UART_C2_TE(1); // Enable UART transmitter - will need if we use transmitter
	//UART1->C2 |= UART_C2_RE(1); // Enable UART receiver - will need if we use receiver
	
}

void UART1_Transmit_DMA() {
	
	
		// transmit pinky data
		while (!(UART1->S1 & UART0_S1_TDRE_MASK))
			;
		UART1->D = LSB(GloveInputs[pinky_index]);
		while (!(UART1->S1 & UART0_S1_TDRE_MASK))
			;
		UART1->D = MSB(GloveInputs[pinky_index]);
		
		// transmit ring data
		while (!(UART1->S1 & UART0_S1_TDRE_MASK))
			;
		UART1->D = LSB(GloveInputs[ring_index]);
		while (!(UART1->S1 & UART0_S1_TDRE_MASK))
			;
		UART1->D = MSB(GloveInputs[ring_index]);
		
		// transmit middle data
		while (!(UART1->S1 & UART0_S1_TDRE_MASK))
			;
		UART1->D = LSB(GloveInputs[middle_index]);
		while (!(UART1->S1 & UART0_S1_TDRE_MASK))
			;
		UART1->D = MSB(GloveInputs[middle_index]);
		
		// transmit index data
		while (!(UART1->S1 & UART0_S1_TDRE_MASK))
			;
		UART1->D = LSB(GloveInputs[index_index]);
		while (!(UART1->S1 & UART0_S1_TDRE_MASK))
			;
		UART1->D = MSB(GloveInputs[index_index]);
		
		// transmit thumb data
		while (!(UART1->S1 & UART0_S1_TDRE_MASK))
			;
		UART1->D = LSB(GloveInputs[thumb_index]);
		while (!(UART1->S1 & UART0_S1_TDRE_MASK))
			;
		UART1->D = MSB(GloveInputs[thumb_index]);
 
}
