#include <MKL25Z4.h>
#include "dma.h"


#define BAUD_RATE (9600)
#define OVERSAMPLE_RATE (25)
#define CLOCK (24000000)


#define RX_PIN 21 //on port E - ALT 4 is UART0_Rx

#define LSB(x) (uint8_t)(x & 0x00FF)
#define MSB(x) (uint8_t)((x & 0xFF00) >> 8)
#define COMBINE(MSB, LSB) (uint16_t)(((uint16_t)MSB << 8) | LSB)

void Init_UART(void); 
void UART0_Receive_DMA(int); 
void CombineReceiveBuffer(void);
void UART0_Receive_char (void);	
void Init_UART1(void); 
	
extern uint8_t receivedbuffer[11];
extern uint16_t receivedADCread[5];
extern uint8_t receivedcal_val;
extern volatile int received_go_signal; 
extern volatile int raise_this_flag;