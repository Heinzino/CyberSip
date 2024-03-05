/*
 * File:   7segment.c
 * Author: heinz
 *
 * Created on February 20, 2024, 12:11 PM
 */


#include <avr/io.h>
#define F_CPU 24000000UL //AVR Clock of 24 MHz 
#include <util/delay.h>
#include <stdint.h>


//DS_pin PA1 (data pin,blue)
//STCP   PA2 (serial clock, white)
//STHP   PA3 (latch/register clock,red)

//DS_pin PA4 (data pin,blue,MOSI)
//STCP   PA6 (serial clock, white, SCK)
//STHP   PA7 (latch clock,red, SS)

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

void serialPulse(void);
void displayDigit(int numToDisplay);
void displayTemperature(int temp);
u8 getData(int temp);


u8 au8_digits[11] = {
  0b011111110, //0
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


u8 test_digits[2] = {
  0b000000000, //0
  0b000000001
};



// |= HIGH
// &= LOW 


int main(void) {
    /* Replace with your application code */
    
    PORTA.DIRSET = 0b11010000; //Set ports to output
    //PORTA.DIRSET = 0b00001110; NON SPI OUTPUT Ports
    //SPI config for HC595 chip
    SPI0.CTRLA = 0b01110011; //Enable SPI, LSB, 4MHz SPI frequency
    SPI0.CTRLB = 0b00000100; //Disable SS to manually use as a latch
    
 
   while (1) {
        for(int i =0; i < 10; ++i){
            displayTemperature(i);
            _delay_ms(100);
        }
    }
}

void serialPulse(void){
    PORTA.OUT &= 0b11111011; //Serial Clock LOW
    _delay_loop_1(1);
    PORTA.OUT |= 0b00000100; //Serial Clock HIGH
    _delay_loop_1(1);
}

void displayDigit(int numToDisplay){
    
    u8 data = getData(numToDisplay); 
    
    PORTA.OUT &= 0b11110111; //Latch LOW

    //Send data serially bigEndian style, update for 16 bit later
    for(int i = 0; i < 8; i++){
        if(data & 0b1000000){
            PORTA.OUT |= 0b00000010; //Data PIN High
        }
        else{
            PORTA.OUT &= 0b11111101; //Data PIN Low
        }
        serialPulse();
        data = data<<1 ;
    }
    
    PORTA.OUT |= 0b00001000; //Latch HIGH to update the register
}

void displayTemperature(int temp){
    u8 data = getData(temp); //Change to 16 bit handle later
    
    PORTA.OUT &= 0b01111111; //Latch Low
    SPI0.DATA = data; 
    while(!(SPI0.INTFLAGS & 0b10000000)); //Wait for SPI data to transfer.
    SPI0.INTFLAGS &= 0b01111111; //Erase interrupt flag
    PORTA.OUT |= 0b1000000; //Latch High

}

u8 getData(int temp){ 
    u8 firstDigit, secondDigit;
    
    secondDigit = au8_digits[(temp % 10)];
    
    if(temp < 0){
        firstDigit = au8_digits[10]; //Negative sign
    }
    else{
        firstDigit = au8_digits[(temp/10)%10];
    }
    
    u16 data = (firstDigit << 8 ) + secondDigit;
    return au8_digits[temp];
}
