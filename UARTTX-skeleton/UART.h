#include <MKL25Z4.h>
#include "dma.h"

#define BAUD_RATE (9600)
#define OVERSAMPLE_RATE (16)
#define CLOCK (48000000)

#define TX_PIN 0 //on port E - ALT 3 is UART1_Tx
#define RX_PIN 1 //on port E - ALT 3 is UART1_Rx

#define LSB(x) (uint8_t)(x & 0x00FF)
#define MSB(x) (uint8_t)((x & 0xFF00) >> 8)
void Init_UART(void); 
void UART1_Transmit_DMA(void); 
 