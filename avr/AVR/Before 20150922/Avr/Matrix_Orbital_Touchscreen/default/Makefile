###############################################################################
# Makefile for the project Matrix_Orbital_Touchscreen
###############################################################################

## General Flags
PROJECT = Matrix_Orbital_Touchscreen
MCU = atmega32
TARGET = Matrix_Orbital_Touchscreen.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99 -DF_CPU=8000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=Matrix_Orbital_Touchscreen.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Include Directories
INCLUDES = -I"N:\Private\Avr\Matrix_Orbital_Touchscreen\..\Webot" 

## Library Directories
LIBDIRS = -L"N:\Private\Avr\Webot" 

## Libraries
LIBS = -lWebbot-ATMega32 -lm -lc 

## Objects that must be built in order to link
OBJECTS = Matrix_Orbital_GLT240128.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) Matrix_Orbital_Touchscreen.hex Matrix_Orbital_Touchscreen.eep Matrix_Orbital_Touchscreen.lss size

## Compile
Matrix_Orbital_GLT240128.o: ../../Webot/Matrix_Orbital_GLT240128.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) Matrix_Orbital_Touchscreen.elf dep/* Matrix_Orbital_Touchscreen.hex Matrix_Orbital_Touchscreen.eep Matrix_Orbital_Touchscreen.lss Matrix_Orbital_Touchscreen.map


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

