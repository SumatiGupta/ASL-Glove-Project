#include <MKL24Z4.h>
#include <stdio.h>

void i2c_init(void);
void i2c_write_byte(void);

void i2c_init(void)
{
	//clock i2c peripheral and PORT C
	SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
	
	//set pins to I2c function
	PORTC->PCR[8] = PORT_PCR_MUX(2);
	PORTC->PCR[9] = PORT_PCR_MUX(2);
	
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

int main(void){
	while(1){
	i2c_init();
	i2c_write_byte();
		
	}
	
}

	
	
	
	
