#include "UART.h"

//UART RX
uint8_t receivedbuffer[11];
uint16_t receivedADCread[5]; 
unsigned char Receive_Done = 0;

void Init_UART(void)
{
	// Let's use UART0 and PTA
	// Enable clock gating for UART0 and Port A
	
	uint16_t sbr; 
	
	//We use Receive data from glove and maybe eventually Transmit (for error check) 
	//Since the UART can only take 8 bits, and we have 5 important 16 bit values that 
	//need to be sent, we would have to fill D reg with each half of an array element
	//making it a total of 5*2 times D needs to be updated. 
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK; 										
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;											
	
	// Make sure transmitter and receiver are disabled before configuring
	UART0->C2 &= ~UART_C2_TE_MASK & ~UART_C2_RE_MASK; 		
	
	// UART 1 has to use Bus Clock, clock can't be configured
	// according to table 5-2 (page 122) of reference manual 
	// and absence of macors
  // but UART0 clock needs to be set - to 48 MHz clock in this case
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);
	//SIM->SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;
	
	// Set pins to UART1 Rx and Tx
	//PORTA->PCR[TX_PIN] |= PORT_PCR_MUX(3); // Tx
	PORTE->PCR[RX_PIN] |= PORT_PCR_MUX(4); // Rx
	
	// Set baud rate and oversampling ratio
	sbr = (uint16_t)((CLOCK)/(BAUD_RATE * OVERSAMPLE_RATE)); 			
	UART0->BDH &= ~UART0_BDH_SBR_MASK;
	UART0->BDH |= UART0_BDH_SBR(sbr>>8);
	UART0->BDL = UART0_BDL_SBR(sbr);
	UART0->C4 |= UART0_C4_OSR(OVERSAMPLE_RATE-1); //UARTLP_C4_OSR(x) is same as UART1_C4_OSR(x) and since C4 reg all UART is same, it should work 	
	
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
  UART0->C1 |= UART0_C1_LOOPS(0); //0: Normal operation - RxD and TxD use separate pins.
	UART0->C1	|= UART0_C1_PE(0);	//Don't use parity bit
																//We can use parity bit but will need to change bit mode to 9 - The 9-bit data mode is typically used with parity to allow eight bits of data plus the parity in the ninth bit
																//When parity is enabled, the bit immediately before the stop bit is treated as the parity bit
																// a little confused about D being only 8 bits long
																// will also need to set PT bit in C1 to parity odd or even and enable interrupt for parity (PEIE) in C3 
																
	// Don't enable interrupts for errors															
	UART0->C3 = UART0_C3_ORIE(0)| UART0_C3_NEIE(0) | UART0_C3_FEIE(0) | UART0_C3_PEIE(0);															

	// Clear error flags
	UART0->S1 |= UART0_S1_OR(1) | UART0_S1_NF(1) | UART0_S1_FE(1) | UART0_S1_PF(1); //To clear these flags write logic one to them 

  UART0->S2 = UART0_S2_MSBF(0) | UART0_S2_RXINV(0); //Send LSB first, do not invert received data
	
	//If Receive Data Register Full Flag (RDRF bit) in UARTx_S1 is high and 
	//setting RIE bit in C2 reg will cause an interrupt - clear flag intially at end of init (based on textbook)
	//UART0->C2 |= UART0_C2_RIE_MASK;
	
	//UART1->C2 |= UART_C2_TE(1); // Enable UART transmitter - will need if we use transmitter
	UART0->C2 |= UART_C2_RE(1); // Enable UART receiver
	
	//UART0->S1 &= ~UART_S1_RDRF_MASK; // Clear the UART RDRF flag mentioned above
	
	//Enable interupt in NVIC 
	//NVIC_SetPriority(UART0_IRQn, 2); // 0, 1, 2, or 3
	//NVIC_ClearPendingIRQ(UART0_IRQn); 
	//NVIC_EnableIRQ(UART0_IRQn);
	
}

void UART0_Receive_DMA(int index) {
		
		while (!(UART0->S1 & UART0_S1_RDRF_MASK))
			;
		receivedbuffer[index] = UART0 -> D;
		
		if(UART0 -> S1 &= UART0_S1_RDRF_MASK)
			Receive_Done++;
 
}

void CombineReceiveBuffer(){
	
	receivedADCread[pinky_index] =  COMBINE(receivedbuffer[1], receivedbuffer[0]);
	receivedADCread[ring_index] =   COMBINE(receivedbuffer[3], receivedbuffer[2]);
	receivedADCread[middle_index] = COMBINE(receivedbuffer[5], receivedbuffer[4]);
	receivedADCread[index_index] =  COMBINE(receivedbuffer[7], receivedbuffer[6]);
	receivedADCread[thumb_index] =  COMBINE(receivedbuffer[9], receivedbuffer[8]);
}