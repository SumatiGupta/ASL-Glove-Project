#include <MKL25Z4.h>
#include "UART.h"
#include "adc.h"
#include "dma.h"

#define A (0)
#define B (1)
#define C (2)
#define D (3)
#define E (4)
#define F (5)
#define G (6)
#define H (7)
#define I (8)
#define J (9)
#define K (10)
#define L (11)
#define M (12)
#define N (13)
#define O (14)
#define P (15)
#define Q (16)
#define R (17)
#define S (18)
#define T (19)
#define U (20)
#define V (21)
#define W (22)
#define X (23)
#define Y (24)
#define Z (25)

#define standing (0)
#define middle	 (1)
#define bent     (2)

#define pinky_LB  (0)
#define pinky_UB  (1)
#define ring_LB   (2)
#define ring_UB   (3)
#define middle_LB (4)
#define middle_UB (5)
#define index_LB  (6)
#define index_UB  (7)
#define thumb_LB  (8)
#define thumb_UB  (9)

extern uint16_t Observed_Standing_values[5];
extern uint16_t Observed_Bent_values[5];
extern uint32_t Standing_Ranges[10];
extern uint32_t Bent_Ranges[10];
extern uint32_t Middle_Ranges[10]; 
extern uint16_t LUT[26][5]; 
extern int pinky_flag, ring_flag, middle_flag, index_flag, thumb_flag; 
extern uint16_t receivedADCread[5];
extern uint8_t receivedcal_val;
extern uint16_t counterstand;
extern uint16_t counterbend;
extern int in;

void ranges(void);
uint32_t Max(uint32_t num1, uint32_t num2); 
uint32_t Min(uint32_t num1, uint32_t num2); 
void LUT_func(void); 
int letter_decode(void); 
void AVG(void);