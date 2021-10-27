#include <MKL25Z4.h>

#define pinky_index (0)
#define ring_index (1)
#define middle_index (2)
#define index_index (3)
#define thumb_index (4)

//variable declarations 
extern int conversion_done_pinky; 
extern int conversion_done_ring; 
extern int conversion_done_middle; 
extern int conversion_done_index; 
extern int conversion_done_thumb;
extern uint16_t  GloveInputs[5];

void dma_init(void); 
void DMA0_IRQHandler(void);
void DMA1_IRQHandler(void); 