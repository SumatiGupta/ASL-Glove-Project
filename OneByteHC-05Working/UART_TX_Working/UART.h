#include <MKL25Z4.h>
#include "dma.h"

//declarations in UART RX

#define BAUD_RATE (9600)
#define OVERSAMPLE_RATE (25)
#define CLOCK (24000000)


#define RX_PIN 21 //on port E - ALT 4 is UART0_Rx

#define LSB(x) (uint8_t)(x & 0x00FF)
#define MSB(x) (uint8_t)((x & 0xFF00) >> 8)
#define GLOVEMASK(x) ((x) >> 16)

void Init_UART(void); 
void UART0_Receive_DMA(int); 
void CombineReceiveBuffer(void);
	
extern uint8_t receivedbuffer[11];
extern uint16_t receivedADCread[5];

//declarations in UART TX 
/*
#include "dma.h"
#define BAUD_RATE (9600)
#define OVERSAMPLE_RATE (16)
#define CLOCK (48000000)*/
#define TX_PIN 20 //on port E - ALT 4 is UART0_Tx
#define COMBINE(MSB, LSB) (uint16_t)(((uint16_t)MSB << 8) | LSB)

extern uint8_t transmitbuffer[11];
extern int shiftedpinky; 
extern int shiftedring; 
extern int shiftedmiddle; 
extern int shiftedindex; 
extern int shiftedthumb; 
extern uint8_t cal_val; 

//void Init_UART(void); 
void UART0_Transmit_DMA(int);
void TransmitBufferCreate(void);
void UART0_Transmit_OneByte(void);