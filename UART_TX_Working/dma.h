#include <MKL25Z4.h>

#define pinky_index (0)
#define ring_index (1)
#define middle_index (2)
#define index_index (3)
#define thumb_index (4)

//variable declarations 
extern volatile int conversion_done; 
extern uint32_t  GloveInputs[8];

void dma_init(void); 
void DMA0_IRQHandler(void);