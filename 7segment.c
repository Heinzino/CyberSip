/*
 * File:   main.c
 * Author: heinz
 *
 * Created on March 11, 2024, 11:12 AM
 */


#include <avr/io.h>
#define F_CPU 24000000UL //AVR Clock of 24 MHz 
#include <util/delay.h>
#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

u8 getFirstDigit(int temp);
void displayDigit(int numToDisplay);

//DS_pin PA4 (data pin,blue,MOSI)
//STCP   PA6 (serial clock, white, SCK)
//STHP   PA7 (latch clock,red, SS)

//Common Cathode configurations for 7seg LEDs A-G
u8 au8_digits[11] = {
  0b01111110, //0
  0b00110000, //1
  0b01101101, //2
  0b01111001, //3
  0b00110011, //4
  0b01011011, //5
  0b01011111, //6
  0b01110000, //7
  0b01111111, //8
  0b01110011, //9
  0b00000001, //Negative sign
};

int main(void) {
    /* Replace with your application code */
    
    PORTA.DIRSET = 0b11010000; //Set ports to output
    PORTA.OUT &= 0b00101111;
    
    SPI0.CTRLA = 0b01100011; //Enable SPI, LSB, 12 MHZ SPI Frequency
    SPI0.CTRLB = 0b00000100; //Disable SS to manually use as a latch

    while (1) {
        displayDigit(2);
        _delay_ms(100);
       /*
        for(int i =0; i < 10; ++i){
            displayDigit(i);
            _delay_ms(300);
        }
       */
      
    }
}

/*
@fn  void displayTemperature(int temp)
@brief Displays two digit temperature onto the seven segment

REQUIRES:
-2 digit temp in degree celsius (-9 to 99)
 
PROMISES:
- Displays two digit temperature onto the seven segment
*/
void displayDigit(int numToDisplay){
    
    u8 secondDigit = au8_digits[(numToDisplay%10)];
    //Send
    
    u8 firstDigit = getFirstDigit(numToDisplay);
    //Send testing just one digit at the bottom
    
    u8 data = au8_digits[numToDisplay];
    char flush_buffer;
    PORTA.OUT &= 0b01111111; //Latch Low
    SPI0.DATA = data; 
    while(!(SPI0.INTFLAGS & 0b10000000)); //Wait for SPI data to transfer.
    flush_buffer = SPI0.DATA; //IF Flag cleared by reading IF then accessing Data
    PORTA.OUT |= 0b10000000; //Latch High

    //Need to make pulse larger HIGH maybe us delay
    //Trying swithing high then low did not work still need a pulse
}


/*
@fn  void getFirstDigit(int temp)
@brief Breaks down first digit of temp to either get negative sign or numeric digit

REQUIRES:
-2 digit temp in degree celsius (-9 to 99)
-au8_digits holds proper data
 
PROMISES:
- Returns proper data to output based on the first digit of temperature; 
 handling negative signs
*/
u8 getFirstDigit(int temp){
    u8 firstDigit;
    if(temp < 0){
        firstDigit = au8_digits[10]; //Negative sign
    }
    else{
        firstDigit = au8_digits[(temp/10)%10];
    }
    return firstDigit;
}


