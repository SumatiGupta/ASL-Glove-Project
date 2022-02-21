#include <MKL25Z4.h>

#define CAL_LED (19) //on port B
#define CAL_SW  (1) //on port A
#define MASK(x) (1UL << (x))

extern uint8_t cal_val; 

void Init_CAL_GPIO(void);
void Read_Switches(void); 
void Light_LED(void); 
void delay(unsigned int length_ms);
