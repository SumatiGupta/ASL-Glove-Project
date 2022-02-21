#include <MKL25Z4.h>
#include "dma.h"

#define BAUD_RATE (9600)
#define OVERSAMPLE_RATE (16)
#define CLOCK (48000000)


#define RX_PIN 21 //on port E - ALT 4 is UART0_Rx
#define TX_PIN 20 //on port E - ALT 4 is UART0_Tx

#define LSB(x) (uint8_t)(x & 0x00FF)
#define MSB(x) (uint8_t)((x & 0xFF00) >> 8)
#define COMBINE(MSB, LSB) (uint16_t)(((uint16_t)MSB << 8) | LSB)

void Init_UART(void); 
void UART0_Receive_DMA(int); 
void CombineReceiveBuffer(void);

void USB_Serial_test(void); 
	
extern uint8_t receivedbuffer[15];
extern uint16_t receivedADCread[5];
extern uint8_t receivedcal_val;
extern uint16_t receivedpressure1;
extern uint16_t receivedpressure2;
