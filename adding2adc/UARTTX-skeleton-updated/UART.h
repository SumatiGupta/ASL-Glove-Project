#include "dma.h"
#define BAUD_RATE (9600)
#define OVERSAMPLE_RATE (16)
#define CLOCK (48000000)

#define TX_PIN 20 //on port E - ALT 4 is UART0_Tx
#define GLOVEMASK(x) ((x) >> 16)
#define LSB(x) (uint8_t)(x & 0x00FF)
#define MSB(x) (uint8_t)((x & 0xFF00) >> 8)

extern uint8_t transmitbuffer[15];
extern int shiftedpinky; 
extern int shiftedring; 
extern int shiftedmiddle; 
extern int shiftedindex; 
extern int shiftedthumb; 
extern int pressure1;
extern int pressure2;
extern uint8_t cal_val; //will be dependent on a calibration push button. This will be updated in a different file that DMA as access to.  
void Init_UART(void); 
void UART0_Transmit_DMA(int);
void TransmitBufferCreate(void); 
 
