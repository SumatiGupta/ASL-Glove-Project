#ifndef DMA_H
#define DMA_H

#include <MKL25Z4.h>
#include "UART.h"
#include "adc.h"
#include "pit.h" 

#define pinky_index (0)
#define ring_index (1)
#define middle_index (2)
#define index_index (3)
#define thumb_index (4)

//variable declarations 
extern volatile int conversion_done; 
extern uint32_t  GloveInputs[8];
extern int while_flag;
extern volatile int conversion_done;
void dma_init(void); 
void DMA0_IRQHandler(void);

#endif 

//Note: added ifndef and endif to get rid of "include nested too deeply"
//error