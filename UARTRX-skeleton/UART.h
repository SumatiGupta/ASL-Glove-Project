#include <MKL25Z4.h>
#include "dma.h"

#define BAUD_RATE (9600)
#define OVERSAMPLE_RATE (16)
#define CLOCK (48000000)

#define TX_PIN 14 //on port A - ALT 3 is UART0_Tx
#define RX_PIN 15 //on port A - ALT 3 is UART0_Rx

#define COMBINE(MSB, LSB) (((uint16_t)MSB << 8) | LSB)

void Init_UART(void); 
void UART0_Receive_DMA(void); 

extern uint16_t receivedbuffer[5];  
