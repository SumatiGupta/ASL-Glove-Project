#include <MKL25Z4.h>
#define MASK(x) (1UL<<(x))

#define LED_RED (1) //on port A
#define LED_YELLOW (2) //on port A
#define	LED_GREEN	(12) //on port A

void Init_SPI(void);
void test_SPIsend(uint8_t d_out); 
void Test_SPI_Loopback(void); 
void Control_RGY_LEDs(int r_on, int g_on, int b_on); 
void Init_Clocks(void);