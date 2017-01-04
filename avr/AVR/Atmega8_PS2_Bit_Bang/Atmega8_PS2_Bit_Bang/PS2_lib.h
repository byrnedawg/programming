
#include <stdlib.h>
#include <math.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#include <avr/pgmspace.h>
#include <stdbool.h>

#define CTRL_CLK        4
#define CTRL_int8_t_DELAY 3

//These are our button constants
#define PSB_SELECT      0x0001
#define PSB_L3          0x0002
#define PSB_R3          0x0004
#define PSB_START       0x0008
#define PSB_PAD_UP      0x0010
#define PSB_PAD_RIGHT   0x0020
#define PSB_PAD_DOWN    0x0040
#define PSB_PAD_LEFT    0x0080
#define PSB_L2          0x0100
#define PSB_R2          0x0200
#define PSB_L1          0x0400
#define PSB_R1          0x0800
#define PSB_GREEN       0x1000
#define PSB_RED         0x2000
#define PSB_BLUE        0x4000
#define PSB_PINK        0x8000
#define PSB_TRIANGLE    0x1000
#define PSB_CIRCLE      0x2000
#define PSB_CROSS       0x4000
#define PSB_SQUARE      0x8000

//Guitar  button constants
#define GREEN_FRET		0x0200
#define RED_FRET		0x2000
#define YELLOW_FRET		0x1000
#define BLUE_FRET		0x4000
#define ORANGE_FRET		0x8000
#define STAR_POWER		0x0100
#define UP_STRUM		0x0010
#define DOWN_STRUM		0x0040
#define WHAMMY_BAR		8

//These are stick values
#define PSS_RX 5
#define PSS_RY 6
#define PSS_LX 7
#define PSS_LY 8

//These are analog buttons
#define PSAB_PAD_RIGHT   9
#define PSAB_PAD_UP      11
#define PSAB_PAD_DOWN    12
#define PSAB_PAD_LEFT    10
#define PSAB_L2          19
#define PSAB_R2          20
#define PSAB_L1          17
#define PSAB_R1          18
#define PSAB_GREEN       13
#define PSAB_RED         14
#define PSAB_BLUE        15
#define PSAB_PINK        16
#define PSAB_TRIANGLE    13
#define PSAB_CIRCLE      14
#define PSAB_CROSS       15
#define PSAB_SQUARE      16


#define SET(x,y) (x|=(1<<y))
#define CLR(x,y) (x&=(~(1<<y)))
#define CHK(x,y) (x & (1<<y))
#define TOG(x,y) (x^=(1<<y))




	bool Button(uint16_t);
	unsigned int ButtonDataint8_t();
	bool NewButtonState();
	bool NewButtonState(unsigned int);
	bool ButtonPressed(unsigned int);
	bool ButtonReleased(unsigned int);
	void read_gamepad();
	void read_gamepad(bool, int8_t);
	int8_t readType();
	int8_t config_gamepad(uint8_t, uint8_t, uint8_t, uint8_t);
	int8_t config_gamepad(uint8_t, uint8_t, uint8_t, uint8_t, bool, bool);
	void enableRumble();
	bool enablePressures();
	int8_t Analog(int8_t);
	unsigned char _gamepad_shiftinout (char);
	unsigned char PS2data[21];
	void sendCommandString(int8_t*, int8_t);
	void reconfig_gamepad();
	unsigned char i;
	unsigned int last_buttons;
	unsigned int buttons;
	uint8_t maskToBitNum(uint8_t);
	uint8_t _clk_mask;
	volatile uint8_t *_clk_oreg;
	uint8_t _cmd_mask;
	volatile uint8_t *_cmd_oreg;
	uint8_t _att_mask;
	volatile uint8_t *_att_oreg;
	uint8_t _dat_mask;
	volatile uint8_t *_dat_ireg;
	unsigned long last_read;
	int8_t read_delay;
	int8_t controller_type;
	bool en_Rumble;
	bool en_Pressures;





