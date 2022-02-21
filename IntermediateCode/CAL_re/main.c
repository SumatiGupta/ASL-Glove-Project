#include "main.h"

int screen_input;

int main (void){
	LUT_func();
	while(1)
	{
		//UART0_Receive_DMA(int);
		ranges(); 
		int screen_input = letter_decode();
	}
}