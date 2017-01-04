###############################################################################
# Makefile for the project TheBoatRx
###############################################################################

## General Flags
PROJECT = TheBoatRx
MCU = atmega168
TARGET = TheBoatRx.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99 -DF_CPU=8000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=TheBoatRx.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Include Directories
INCLUDES = -I"C:\Users\gregory.byrne\Desktop\Projects\Electrical Engineering\AVR\Avr\AVR Working\TheBoatRx\..\Webot" 

## Library Directories
LIBDIRS = -L"C:\Users\gregory.byrne\Desktop\Projects\Electrical Engineering\AVR\Avr\AVR Working\Webot" 

## Libraries
LIBS = -lWebbot-ATMega168 -lm -lc 

## Objects that must be built in order to link
OBJECTS = TheBoatRx.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) TheBoatRx.hex TheBoatRx.eep TheBoatRx.lss size

## Compile
TheBoatRx.o: ../TheBoatRx.c
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
	-rm -rf $(OBJECTS) TheBoatRx.elf dep/* TheBoatRx.hex TheBoatRx.eep TheBoatRx.lss TheBoatRx.map


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

