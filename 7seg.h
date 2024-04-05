#ifndef SEG_H
#define SEG_H

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

u8 getFirstDigit(int temp);
void displayTemp(int numToDisplay);

//DS_pin PA4 (data pin,blue,MOSI)
//STCP   PA6 (serial clock, white, SCK)
//STHP   PA7 (latch clock,red, SS)

#endif

