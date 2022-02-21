#include <MKL24Z4.h>
#include <stdio.h>

void i2c_init(void);
void i2c_write_byte(void);
void Init_LCD(void);
void Delay(volatile unsigned int time_del);
void clear_display(void);

void i2c_init(void)
{
	//clock i2c peripheral and PORT C
	SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
	
	//set pins to I2c function
	PORTC->PCR[8] = PORT_PCR_MUX(2); //SCL
	PORTC->PCR[9] = PORT_PCR_MUX(2); //SDA
	
	//set 400k baud
	//baud = bus freq/(scl_div+mul)
	//24MHz/400kHz = 60; icr=0x11 sets scl_div to 56
	I2C0->F = I2C_F_ICR(0x11) | I2C_F_MULT(0);
	
	//enable I2C and set to master mode
	I2C0->C1 |= (I2C_C1_IICEN_MASK);
	
	//select high drive mode
	I2C0->C2 |= (I2C_C2_HDRS_MASK);
	
}

void i2c_write_byte(void){
	
	I2C0->C1 |= I2C_C1_TX_MASK;
	I2C0->C1 |= I2C_C1_MST_MASK;
	I2C0->D = 0x27;
	while ((I2C0->S & I2C_S_IICIF_MASK) == 0) {}\
I2C0->S |= I2C_S_IICIF_MASK;
		I2C0->D = 0x3B;
	while ((I2C0->S & I2C_S_IICIF_MASK) == 0) {}\
I2C0->S |= I2C_S_IICIF_MASK;
		I2C0->D = 0xFF;
	while ((I2C0->S & I2C_S_IICIF_MASK) == 0) {}\
I2C0->S |= I2C_S_IICIF_MASK;
		
I2C0->C1 &= ~I2C_C1_MST_MASK;

}
void Init_LCD(void){
		
	//code to intiliaze LCD according to datasheet. 
	//  Wait for power stabilization 500ms:
	Delay(500);
	
	//Function set: 00111(N)(F)(FT1)(FT0)
	//  N = lines: N=1 is 2 lines
  //  F = Font: 0 = 5x8, 1 = 5x10
  //  FT = Font Table:
  //  FT=00 is English/Japanese ~"standard" for character LCDs
  //  FT=01 is Western European I fractions, circle-c some arrows
  //  FT=10 is English/Russian
  //  FT=11 is Western European II my favorite, arrows, Greek letters
  I2C0->C1 |= I2C_C1_TX_MASK;
	I2C0->C1 |= I2C_C1_MST_MASK;
	I2C0->D = 0x27;
	while ((I2C0->S & I2C_S_IICIF_MASK) == 0) {}\
  I2C0->S |= I2C_S_IICIF_MASK;
		I2C0->D = 0x3B;
	while ((I2C0->S & I2C_S_IICIF_MASK) == 0) {}\
		I2C0->S |= I2C_S_IICIF_MASK;
		
	I2C0->D = 0x3B; 
	
	//Display On/Off Control RS=0,R/W=0
  // 0000 1DCB
  //  D = Display On
  //  C = Cursor On
  //  B = Cursor Blink
  
	I2C0->D = (0x0E);
		while ((I2C0->S & I2C_S_IICIF_MASK) == 0) {}\
I2C0->S |= I2C_S_IICIF_MASK;
		
I2C0->C1 &= ~I2C_C1_MST_MASK;
	
	
	//Graphic vs character mode setting, RS=0,R/W=0
  // 0001 GP11
  //  G = Mode: 1=graphic, 0=character
  //  C = Power: 1=0n, 0=off
	//while(!(SPI0->S & SPI_S_SPTEF_MASK))
	//	;  //Wait for transmit buffer empty 
	//SPI0->D = 0x17; 
	
	clear_display(); 
	
	//Display home
  I2C0->C1 |= I2C_C1_TX_MASK;
	I2C0->C1 |= I2C_C1_MST_MASK;
	I2C0->D = 0x27;
	while ((I2C0->S & I2C_S_IICIF_MASK) == 0) {}\
  I2C0->S |= I2C_S_IICIF_MASK;
		I2C0->D = 0x3B;
	while ((I2C0->S & I2C_S_IICIF_MASK) == 0) {}\
		I2C0->S |= I2C_S_IICIF_MASK;
	I2C0->D = (0x02);
		while ((I2C0->S & I2C_S_IICIF_MASK) == 0) {}\
I2C0->S |= I2C_S_IICIF_MASK;
		
I2C0->C1 &= ~I2C_C1_MST_MASK;
 
 //Entry Mode Set RS=0,R/W=0
  // 0000 01IS
  //  I = Increment/or decrement
  //  S = Shift(scroll) data on line
  
		I2C0->C1 |= I2C_C1_TX_MASK;
	I2C0->C1 |= I2C_C1_MST_MASK;
	I2C0->D = 0x27;
	while ((I2C0->S & I2C_S_IICIF_MASK) == 0) {}\
  I2C0->S |= I2C_S_IICIF_MASK;
		I2C0->D = 0x3B;
	while ((I2C0->S & I2C_S_IICIF_MASK) == 0) {}\
		I2C0->S |= I2C_S_IICIF_MASK;
	I2C0->D = (0x06);
	//stop
	while ((I2C0->S & I2C_S_IICIF_MASK) == 0) {}\
I2C0->S |= I2C_S_IICIF_MASK;
		
I2C0->C1 &= ~I2C_C1_MST_MASK;
	
	Delay(2000);
  clear_display();
  Delay(2000);
}
	

void Delay(volatile unsigned int time_del){
	while(time_del--){
		;
	}
}

void clear_display(void)
{
  //Display Clear RS=0,R/W=0
  // 0000 0001
  while ((I2C0->S & I2C_S_IICIF_MASK) == 0) {}\
  I2C0->S |= I2C_S_IICIF_MASK;
		I2C0->D =0x01;
  Delay(60000);
}


int main(void){
	while(1){
	i2c_init();
	//Init_LCD();	
	i2c_write_byte();
	}
	
}


	
	
	
	
