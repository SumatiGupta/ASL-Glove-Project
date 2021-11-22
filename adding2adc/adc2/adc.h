#include <MKL25Z4.h>

#define ADCpinky 				(0) //port B - ADC0_SE8, Channel: AD8, ADC Channel:01000 
#define ADCring 				(1) //port B - ADC0_SE9, Channel: AD9, ADC Channel:01001
#define ADCmiddle 			(2) //port B - ADC0_SE12, Channel: AD12, ADC Channel:01100
#define ADCindex 				(3) //port B - ADC0_SE13, Channel: AD13, ADC Channel:01101
#define ADCthumb 				(20) //port E - ADC0_DP0/SE0, Channel: DAD0, ADC Channel:00000

#define Pressure_1      (22) //port E - ADC0_DM0/ADC0_SE4a, Channel:AD4a, ADC Channel"00100
#define Pressure_2 			(23)//port E - ADC0_DP3/ADC0_SE3, Channel: DAD3, ADC Channel:00011 

#define ADCpinky_CH     (0x8) 
#define ADCring_CH 			(0x9) 
#define ADCmiddle_CH 		(0xc) 
#define ADCindex_CH 		(0xd)
#define ADCthumb_CH 		(0x0)

#define Pressure1_CH     (0x3)
#define Pressure2_CH		 (0x4)

#define ADC_set_MSB 		(0x8000)

extern int CH_idx[8]; 

void adc_init(void);
void adc_read(int i); 
int internal_calibration_adc(void); 
