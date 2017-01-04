

#define	I2C_LIS302DL	0x1d	// LIS302DL accelerometer default I2C address.
#define outX			0x01
#define outY			0x03
#define outZ			0x05
#define statusReg		0x00

#include	"i2cmaster.h"
  
void LIS302DL_init (void);
int LIS302DL_read (unsigned char axe);

