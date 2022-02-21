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

void Init_14Segment(void){
//Enable Clock to port A and C
SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTC_MASK);
	PORTA->PCR[LED_A] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[LED_A] |= PORT_PCR_MUX(1);
	PORTA->PCR[LED_B] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[LED_B] |= PORT_PCR_MUX(1);
	PORTA->PCR[LED_C] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[LED_C] |= PORT_PCR_MUX(1);
	PORTA->PCR[LED_D] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[LED_D] |= PORT_PCR_MUX(1);
	PORTA->PCR[LED_E] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[LED_E] |= PORT_PCR_MUX(1);
	PORTA->PCR[LED_F] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[LED_F] |= PORT_PCR_MUX(1);
	PORTA->PCR[LED_G] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[LED_G] |= PORT_PCR_MUX(1);
	PORTC->PCR[LED_H] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[LED_H] |= PORT_PCR_MUX(1);
	PORTC->PCR[LED_K] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[LED_K] |= PORT_PCR_MUX(1);
	PORTC->PCR[LED_M] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[LED_M] |= PORT_PCR_MUX(1);
	PORTC->PCR[LED_N] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[LED_N] |= PORT_PCR_MUX(1);
	PORTC->PCR[LED_P] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[LED_P] |= PORT_PCR_MUX(1);
	PORTC->PCR[LED_S] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[LED_S] |= PORT_PCR_MUX(1);
	PORTC->PCR[LED_R] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[LED_R] |= PORT_PCR_MUX(1);
	PORTC->PCR[LED_T] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[LED_T] |= PORT_PCR_MUX(1);
	PORTC->PCR[LED_U] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[LED_U] |= PORT_PCR_MUX(1);
	
		//Set LED bits to outputs
	PTA->PDDR |= MASK(LED_A) | MASK(LED_B) | MASK(LED_C) | MASK(LED_D) | MASK(LED_E) | MASK(LED_F)| MASK(LED_G);
  PTC->PDDR |= MASK(LED_H) | MASK(LED_K) | MASK(LED_M) | MASK(LED_N) | MASK(LED_P) | MASK(LED_S)| MASK(LED_R) | MASK(LED_T)| MASK(LED_U);
	
	//Turn off all LED's off initially
	PTA->PSOR=MASK(LED_A);
	PTA->PSOR=MASK(LED_B);
	PTA->PSOR=MASK(LED_C);
	PTA->PSOR=MASK(LED_D);
	PTA->PSOR=MASK(LED_E);
	PTA->PSOR=MASK(LED_F);
	PTA->PSOR=MASK(LED_G);
	PTC->PSOR=MASK(LED_H);
	PTC->PSOR=MASK(LED_K);
	PTC->PSOR=MASK(LED_M);
	PTC->PSOR=MASK(LED_N);
	PTC->PSOR=MASK(LED_P);
	PTC->PSOR=MASK(LED_S);
	PTC->PSOR=MASK(LED_R);
	PTC->PSOR=MASK(LED_T);
	PTC->PSOR=MASK(LED_U);	
}

void control_LEDs(int A_on, int B_on, int C_on, int D_on, int E_on, int F_on, int G_on,int H_on, int K_on, int M_on, int N_on, int P_on, int S_on, int R_on, int T_on, int U_on)
{
	if (A_on)
		PTA->PCOR = MASK(LED_A);
	else 
		PTA->PSOR = MASK(LED_A);
	if (B_on)
		PTA->PCOR = MASK(LED_B);
	else 
		PTA->PSOR = MASK(LED_B);
	if (C_on)
		PTA->PCOR = MASK(LED_C);
	else 
		PTA->PSOR = MASK(LED_C);
	if (D_on)
		PTA->PCOR = MASK(LED_D);
	else 
		PTA->PSOR = MASK(LED_D);
	if (E_on)
		PTA->PCOR = MASK(LED_E);
	else 
		PTA->PSOR = MASK(LED_E);
	if (F_on)
		PTA->PCOR = MASK(LED_F);
	else 
		PTA->PSOR = MASK(LED_F);
	if (G_on)
		PTA->PCOR = MASK(LED_G);
	else 
		PTA->PSOR = MASK(LED_G);
  if (H_on)
		PTC->PCOR = MASK(LED_H);
	else 
		PTC->PSOR = MASK(LED_H);
	if (K_on)
		PTC->PCOR = MASK(LED_K);
	else 
		PTC->PSOR = MASK(LED_K);
	if (M_on)
		PTC->PCOR = MASK(LED_M);
	else 
		PTC->PSOR = MASK(LED_M);
	if (N_on)
		PTC->PCOR = MASK(LED_N);
	else 
		PTC->PSOR = MASK(LED_N);
	if (P_on)
		PTC->PCOR = MASK(LED_P);
	else 
		PTC->PSOR = MASK(LED_P);
	if (S_on)
		PTC->PCOR = MASK(LED_S);
	else 
		PTC->PSOR = MASK(LED_S);
	if (R_on)
		PTC->PCOR = MASK(LED_R);
	else 
		PTC->PSOR = MASK(LED_R);
	if (T_on)
		PTC->PCOR = MASK(LED_T);
	else 
		PTC->PSOR = MASK(LED_T);
	if (U_on)
		PTC->PCOR = MASK(LED_U);
	else 
		PTC->PSOR = MASK(LED_U);
	
}
void Display_Alphabet(int input){
	
	if(input == 0)
	{
		control_LEDs(1,1,1,1,0,0,1,1,0,0,0,1,0,0,0,1);
	}
	else if(input == 1)
	{
		control_LEDs(1,1,1,1,1,1,0,0,0,1,0,1,0,1,0,0);
	}
	else if(input == 2)
	{
		control_LEDs(1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0);
	}
	else if(input == 3)
	{
		control_LEDs(1,1,1,1,1,1,0,0,0,1,0,0,0,1,0,0);
	}
	else if(input == 4)
	{
		control_LEDs(1,1,0,0,1,1,1,1,0,0,0,1,0,0,0,1);
	}
	else if(input == 5)
	{
		control_LEDs(1,1,0,0,0,0,1,1,0,0,0,1,0,0,0,1);
	}
	else if(input == 6)
	{
		control_LEDs(1,1,0,1,1,1,1,1,0,0,0,1,0,0,0,0);
	}
	else if(input == 7)
	{
		control_LEDs(0,0,1,1,0,0,1,1,0,0,0,1,0,0,0,1);
	}
	else if(input == 8)
	{
		control_LEDs(1,1,0,0,1,1,0,0,0,1,0,0,0,1,0,0);
  }
  else if(input == 9)
  {
    control_LEDs(1,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0);
  }
  else if(input == 10)
  {
    control_LEDs(0,0,0,0,0,0,1,1,0,0,1,0,1,0,0,1);
  }
  else if(input == 11)
  {
		control_LEDs(0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0);
	}
	else if(input == 12)
	{
		control_LEDs(0,0,1,1,0,0,1,1,1,0,1,0,0,0,0,0);
	}
	else if(input == 13)
	{
		control_LEDs(0,0,1,1,0,0,1,1,1,0,0,0,1,0,0,0);
	}
	else if(input == 14)
	{
		control_LEDs(1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0);
	}
	else if(input == 15)
	{
		control_LEDs(1,1,1,0,0,0,1,1,0,0,0,1,0,0,0,1);
	}
	else if(input == 16)
	{
		control_LEDs(1,1,1,1,1,1,1,1,0,0,0,0,1,0,0,0);
	}
	else if(input == 17)
	{
		control_LEDs(1,1,1,0,0,0,1,1,0,0,0,1,1,0,0,1);
	}
	else if(input == 18)
	{
		control_LEDs(1,1,0,1,1,1,0,1,0,0,0,1,0,0,0,1);
	}
	else if(input == 19)
	{
		control_LEDs(1,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0);
	}
	else if(input == 20)
	{
		control_LEDs(0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0);
	}
	else if(input == 21)
	{
		control_LEDs(0,0,0,0,0,0,1,1,0,0,1,0,0,0,1,0);
	}
	else if(input == 22)
	{
		control_LEDs(0,0,1,1,0,0,1,1,0,0,0,0,1,0,1,0);
	}
	else if(input == 23)
	{
		control_LEDs(0,0,0,0,0,0,0,0,1,0,1,0,1,0,1,0);
	}
	else if(input == 24)
	{
		control_LEDs(0,0,1,1,1,1,0,1,0,0,0,1,0,0,0,1);
	}
	else if(input == 25)
	{
		control_LEDs(1,1,0,0,1,1,0,0,0,0,1,0,0,0,1,0);
	}
}
 
int main (void){
	while(1){
		Init_14Segment();
		//control_LEDs(1,1,0,0,1,1,0,0,0,0,1,1,0,0,0,0);
		Display_Alphabet(0);
	}
}
	
	
	
	

