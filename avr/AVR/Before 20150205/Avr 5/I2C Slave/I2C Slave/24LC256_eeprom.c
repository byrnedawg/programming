//
// 24LC256_eeprom.c
// 
// Demonstrate writing/reading serial eeprom via TWI/I2C
//
 
#include <avr/interrupt.h>
#include <stdbool.h>
#include <avr/pgmspace.h>
//#include "../libnerdkits/lcd.h"
#include "../libNoter/TWI.h"
 
// Define format of eeprom data block.
typedef struct {
    bool        need_initalization;
    char        author[6];
    uint16_t    read_count;
    uint8_t     brightness;
    float       version;
} EEPROM_DATA;
 
// LCD stream file - enable printf in functions outside of main()
FILE lcd_stream;
 
// prototype local functions
void eeprom_init();
EEPROM_DATA read_eeprom(uint16_t memory_address);
void write_eeprom(uint16_t memory_address, EEPROM_DATA *w_data);
void show_eeprom_data(EEPROM_DATA *data);
void handle_TWI_result(uint8_t return_code);
 
// -----------------------------------------------------------------
int main() {
    // Initialize the lcd
    lcd_init();
    fdev_setup_stream(&lcd_stream, lcd_putchar, 0, _FDEV_SETUP_WRITE); 
    lcd_clear_and_home();
 
    // initialize eeprom and TWI/I2C
    eeprom_init();
 
    // specify 7 bit device address of eeprom chip
    #define EEPROM_DEVICE_ID 0b1010000
    fprintf_P(&lcd_stream, PSTR("24LC256 ADDR: %02X"), EEPROM_DEVICE_ID);
 
    // specify eeprom memory address for data storage
    #define EEPROM_DATA_ADDRESS 0x0000
 
    // create local copy of eeprom data
    EEPROM_DATA e_data;
 
    // read eeprom contents at location 0000
    e_data = read_eeprom(0);
 
    // show what we read from the eeprom -
    // note: the very first read on a new eeprom
    // will show uninitalized data
    show_eeprom_data(&e_data);
 
    // process data
    if(e_data.need_initalization){
        // set all data item values if first time
        e_data.need_initalization = false;
        strcpy_P(e_data.author, PSTR("Noter"));
        e_data.read_count = 1;
        e_data.brightness = 0x33;
        e_data.version = 1.01;
    } else {
        // check contents against the values written when initializing
        if((e_data.version != 1.01)||
            (strcmp_P(e_data.author, PSTR("Noter")) !=0)||
            (e_data.brightness != 0x33)){
                    lcd_line_four();
                    fprintf_P(&lcd_stream, PSTR("DATA ERROR - ")); 
                    while(true);    // and freeze
                } else {
                    // increment read_count 
                    e_data.read_count += 1;
                }
    }
 
    // write data to eeprom memory at location 0000
    write_eeprom(0, &e_data);
 
    // and show the read count
    lcd_line_two();
    fprintf_P(&lcd_stream, PSTR("READ COUNT = %d"), e_data.read_count);
 
    // done
    while(true);
}
// -----------------------------------------------------------------
 
// Define eeprom commands according to 24LC256 datasheet
typedef struct {
    uint8_t     high_byte;
    uint8_t     low_byte;
    EEPROM_DATA eeprom_data;
} WRITE_EEPROM;
 
typedef struct {
    uint8_t     high_byte;
    uint8_t     low_byte;
} SET_EEPROM_ADDRESS;
 
typedef struct {
    EEPROM_DATA eeprom_data;
} READ_EEPROM;
 
// Create structure pointers for the TWI/I2C buffer
WRITE_EEPROM            *p_write_eeprom;
SET_EEPROM_ADDRESS      *p_set_eeprom_address;
READ_EEPROM             *p_read_eeprom;
 
// Create TWI/I2C buffer, size to largest command
char    TWI_buffer[sizeof(WRITE_EEPROM)];
 
void eeprom_init(){
    // Specify startup parameters for the TWI/I2C driver
    TWI_init(   F_CPU,                      // clock frequency
                100000L,                    // desired TWI/IC2 bitrate
                TWI_buffer,                 // pointer to comm buffer
                sizeof(TWI_buffer),         // size of comm buffer
                &handle_TWI_result          // pointer to callback function
                );
 
    // Enable interrupts
    sei();
 
    // Set our structure pointers to the TWI/I2C buffer
    p_write_eeprom = (WRITE_EEPROM *)TWI_buffer;
    p_set_eeprom_address = (SET_EEPROM_ADDRESS *)TWI_buffer;
    p_read_eeprom = (READ_EEPROM *)TWI_buffer;
 
}
 
EEPROM_DATA read_eeprom(uint16_t memory_address){
    // send 'set memory address' command to eeprom and then read data
    while(TWI_busy);
    p_set_eeprom_address->high_byte = memory_address >> 8;
    p_set_eeprom_address->low_byte = memory_address & 0x0F;
    TWI_master_start_write_then_read(   EEPROM_DEVICE_ID,               // device address of eeprom chip
                                        sizeof(SET_EEPROM_ADDRESS),     // number of bytes to write
                                        sizeof(EEPROM_DATA)             // number of bytes to read
                                        );
 
    // nothing else to do - wait for the data
    while(TWI_busy);
    // return the data
    return(p_read_eeprom->eeprom_data);
}
 
// write eeprom - note: page boundaries are not considered in this example
void write_eeprom(uint16_t memory_address, EEPROM_DATA *w_data){
    while(TWI_busy);
    p_write_eeprom->high_byte = EEPROM_DATA_ADDRESS >> 8;
    p_write_eeprom->low_byte = EEPROM_DATA_ADDRESS & 0x0F;
    p_write_eeprom->eeprom_data = *w_data;
    TWI_master_start_write(     EEPROM_DEVICE_ID,       // device address of eeprom chip
                                sizeof(WRITE_EEPROM)    // number of bytes to write
                                );
 
}
 
// optional callback function for TWI/I2C driver
void handle_TWI_result(uint8_t return_code){
    if(return_code!=TWI_success){
        lcd_line_four();
        fprintf_P(&lcd_stream, PSTR("I2C ERROR - %02X"), return_code); 
    }
}
 
// format and display eeprom data on lcd
void show_eeprom_data(EEPROM_DATA *data){
    lcd_line_three();
    fprintf_P(&lcd_stream, PSTR("%1d %s %04X %02X %4.2f"), 
        data->need_initalization,
        data->author,
        data->read_count,
        data->brightness,
        data->version       
        ); 
}