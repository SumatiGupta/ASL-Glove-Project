#ifndef UART_H
#define UART_H

#include <MKL25Z4.h>
#include "dma.h"
#include "read.h"

//declarations in UART RX

#define BAUD_RATE     	(9600)
#define OVERSAMPLE_RATE (25)
#define CLOCK     			(24000000)

#define TX_PIN          (2) //on port A - ALT 2 is UART0_Tx

#define LSB(x)            (uint8_t)(x & 0x00FF)
#define MSB(x)            (uint8_t)((x & 0xFF00) >> 8)
#define GLOVEMASK(x)      ((x) >> 16)
#define COMBINE(MSB, LSB) (uint16_t)(((uint16_t)MSB << 8) | LSB)

void Init_UART(void); 
void UART0_Transmit_DMA(int);
void TransmitBufferCreate(void);
void UART1_Transmit(int value); 
void UART0_Transmit_OneByte(void);
void Init_UART1(void); 


extern uint8_t transmitbuffer[11];
extern int shiftedpinky; 
extern int shiftedring; 
extern int shiftedmiddle; 
extern int shiftedindex; 
extern int shiftedthumb; 
//extern uint8_t cal_val; 

#endif 

//Note: added ifndef and endif to get rid of "include nested too deeply"
//error
