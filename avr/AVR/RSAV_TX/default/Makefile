################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS := 


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../RSAV_TX.c


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
RSAV_TX.o

OBJS_AS_ARGS +=  \
RSAV_TX.o

C_DEPS +=  \
RSAV_TX.d

C_DEPS_AS_ARGS +=  \
RSAV_TX.d

OUTPUT_FILE_PATH +=RSAV_TX.elf

OUTPUT_FILE_PATH_AS_ARGS +=RSAV_TX.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+=  \
../../../../../../webbotavrclib-1.36/libWebbot-ATMega168.a

# AVR32/GNU C Compiler



./%.o: .././%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.3.3
	$(QUOTE)C:\WinAVR-20100110\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DF_CPU=8000000UL  -I"..\..\..\..\..\..\webbotavrclib-1.36"  -O0 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -mmcu=atmega168 -c -gdwarf-2 -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP)
	@echo Building target: $@
	@echo Invoking: AVR/GNU Linker : 4.3.3
	$(QUOTE)C:\WinAVR-20100110\bin\avr-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="RSAV_TX.map" -Wl,--start-group -Wl,-lWebbot-ATMega168  -Wl,--end-group -Wl,-L"..\..\..\..\..\..\webbotavrclib-1.36"  -Wl,--gc-sections -mmcu=atmega168  
	@echo Finished building target: $@
	"C:\WinAVR-20100110\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  "RSAV_TX.elf" "RSAV_TX.hex"
	"C:\WinAVR-20100110\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "RSAV_TX.elf" "RSAV_TX.eep" || exit 0
	"C:\WinAVR-20100110\bin\avr-objdump.exe" -h -S "RSAV_TX.elf" > "RSAV_TX.lss"
	"C:\WinAVR-20100110\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures "RSAV_TX.elf" "RSAV_TX.srec"
	"C:\WinAVR-20100110\bin\avr-size.exe" "RSAV_TX.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "RSAV_TX.elf" "RSAV_TX.a" "RSAV_TX.hex" "RSAV_TX.lss" "RSAV_TX.eep" "RSAV_TX.map" "RSAV_TX.srec" "RSAV_TX.usersignatures"
	