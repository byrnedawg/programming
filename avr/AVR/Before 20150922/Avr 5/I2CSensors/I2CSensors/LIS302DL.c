
#include <util/delay.h>
#include	"LIS302DL.h"

void LIS302DL_init (void)
{
	i2c_start(I2C_LIS302DL + I2C_WRITE);
	i2c_write(0x11); // CTRL_REG2 (21h)
	i2c_write(0x80);              // send 0x80, enable auto resets
	i2c_stop();
	_delay_ms(15);
	i2c_start(I2C_LIS302DL + I2C_WRITE);
	i2c_write(0x10);              // cntrl register1
	i2c_write(0x01);                 // send 0x01, active mode
	i2c_stop();

}

int LIS302DL_read (unsigned char axe )
{
	int axe_val;
	unsigned char tmp;

	i2c_start(I2C_LIS302DL + I2C_WRITE);
	//i2c_write(statusReg);
	i2c_write(axe);
	i2c_stop();
   _delay_us(2);
   
	//i2c_start(I2C_LIS302DL + I2C_WRITE);
	//i2c_write(axe);
	//i2c_stop();
   
	i2c_start(I2C_LIS302DL + I2C_READ);
	tmp = i2c_readNak();
	i2c_stop();

	if (tmp > 127) //If axis value is negative,
	{
		axe_val = 256 - tmp;
		axe_val ^= 0xFFFF; //One's complement of axe_val.
	}   
   else
   	 axe_val = tmp;

	return (axe_val);

}