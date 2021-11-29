#include "main.h"

int ADC_is_reading_cal1 = 1000; //this flag is high till we are in cal = 1 mode
int ADC_is_reading_cal2 = 1000; 
//uint16_t receivedADCread[5]= {0x0000,0x0002,0x0004,0x0006,0x0008}; 

int main (void){
	LUT_func();
	Init_14Segment();
	
	receivedADCread[0] = 0x02E4;
	receivedADCread[1] = 0x01B4;
	receivedADCread[2] = 0x04A3;
	receivedADCread[3] = 0x01C3;
	receivedADCread[4] = 0x0AA5;
	
	while(1)
	{
		receivedcal_val = 1;
		while(ADC_is_reading_cal1)
		{
			receivedADCread[0] = 0x8EE4;
			receivedADCread[1] = 0xB1B4;
			receivedADCread[2] = 0xB4A3;
			receivedADCread[3] = 0xB1C3;
			receivedADCread[4] = 0xAAA5;
			ranges(); 
			ADC_is_reading_cal1--;
		}
		
		receivedcal_val = 2;
		while(ADC_is_reading_cal2)
		{
			receivedADCread[0] = 0x4AE4;
			receivedADCread[1] = 0x02B4;
			receivedADCread[2] = 0x7EA3;
			receivedADCread[3] = 0x9123;
			receivedADCread[4] = 0x52A5;
			ranges(); 
			ADC_is_reading_cal2--;
		}
		receivedcal_val = 0; 
		if (receivedcal_val == 0)
		{
			AVG(); 
			receivedADCread[0] = 0x8EFF;
			receivedADCread[1] = 0xBCF3;
			receivedADCread[2] = 0xCA00;
			receivedADCread[3] = 0xCFE3;
			receivedADCread[4] = 0x4F32;
			
			int input = letter_decode();
			in = input;
			Display_Alphabet(input);
		}
	}

	
	/*while(1)
	{ 
		receivedcal_val = 1;
		receivedADCread[0] = 0x02E4;
		receivedADCread[1] = 0x01B4;
		receivedADCread[2] = 0x04A3;
		receivedADCread[3] = 0x01C3;
		receivedADCread[4] = 0x0AA5;
		ranges(); //updates standing ranges 
		//AVG(); 
		
		receivedcal_val = 2;
		receivedADCread[0] = 0x0AE4;
		receivedADCread[1] = 0x0AB4;
		receivedADCread[2] = 0x0EA3;
		receivedADCread[3] = 0x0123;
		receivedADCread[4] = 0x02A5;
		
		ranges(); //updates bent ranges
		
		
		//Calculate ADCreceived 
		receivedcal_val = 0; 
		AVG(); //
		
		receivedADCread[0] = 0x1F40;
		receivedADCread[1] = 0x1858;
		receivedADCread[2] = 0x1770;
		receivedADCread[3] = 0x1388;
		receivedADCread[4] = 0x1388;
		
		int input = letter_decode();
		in = input;
		Display_Alphabet(input);
		
	}*/
}
