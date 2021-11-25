#include <MKL25Z4.h>
#include "adc.h"

extern int counter; 

void pit_init(void);
void PIT_IRQHandler(void); 
