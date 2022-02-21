#include <MKL25Z4.h>

#define MASK(x) (1UL << (x))
#define LED_A (1) //on port A
#define LED_B (2) //on port A
#define LED_C (4) //on port A
#define LED_D (5) //on port A
#define LED_E (12) //on port A
#define LED_F (13) //on port A
#define LED_G (16) //on port A

#define LED_H (7)//on port C
#define LED_K (0)//on port C
#define LED_M (3)//on port C
#define LED_N (4) //on port C
#define LED_P (5)//on port C
#define LED_R (6) //on port C
#define LED_S (10)//on port C
#define LED_T (8) //on port C
#define LED_U (9)//on port C

void Init_14Segment(void);
void control_LEDs(int A_on, int B_on, int C_on, int D_on, int E_on, int F_on, int G_on,int H_on, int K_on, int M_on, int N_on, int P_on, int R_on, int S_on, int T_on, int U_on);
void Display_Alphabet(int input);

