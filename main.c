/*
 * File:   main.c
 * Author: heinz
 *
 * Created on March 19, 2024, 10:02 AM
 */


#include <avr/io.h>
#include "7seg.h"
#include "rt.h"

int main(void) {
    /* Replace with your application code */
    
    
    //PIN LAYOUT
    /*
     DS_pin PA4 (data pin,blue,MOSI)
     STCP   PA6 (serial clock, white, SCK)
     STHP   PA7 (latch clock,red, SS)
     Thermistor Input PD1
     Relay Output PA1
     */
    
    PORTA.DIRSET = 0b11010010; //Set ports to output
    PORTA.OUT &= 0b00101111;
    
    SPI0.CTRLA = 0b01100011; //Enable SPI, LSB, 12 MHZ SPI Frequency
    SPI0.CTRLB = 0b00000100; //Disable SS to manually use as a latch
    
    
    // Enable global interrupts. *Bit  7 is global interupt bit
    SREG = 0b10000000;
            
    // Set the ADC reference level to VDD. *
    VREF.ADC0REF = 0b10000101;
        
    // Enable the ADC interrupt. *
    ADC0.INTCTRL = 0b00000001;
        
    //Single Ended, Right Adjusted, 12 bit-resolution, Free Running
    ADC0.CTRLA = 0b00000011;
        
    // Select minimum clock divide. Div  by 2. *
    ADC0.CTRLC = 0x00;
        
    enum multiplexVals { OnChipTemperatureSensor = 0x42, PD1 = 0b00000010};
        
    u8 au8_multiplexSelectorVals[] = {
            OnChipTemperatureSensor,
            PD1,
    };
        
    ADC0.MUXPOS = au8_multiplexSelectorVals[1];
        
    // Start conversion.
    ADC0.COMMAND = 0x01;
    
    int oldTemp = 0, newTemp = 0;
    if(ADC0.INTFLAGS & 0x01){ //Result is ready from ADC
        u32 u32_adcReading = ADC0.RES;
        ADC0.INTFLAGS &= 0x01; //Clear flag to read next time
        oldTemp = getTemperatureFromThermistor(u32_adcReading);
    }
    
    while (1) {
        
        if(ADC0.INTFLAGS & 0x01){ //Result is ready from ADC
            u32 u32_adcReading = ADC0.RES;
            ADC0.INTFLAGS &= 0x01; //Clear flag to read next time
            
            newTemp = getTemperatureFromThermistor(u32_adcReading);
            
            if((newTemp-oldTemp) >= 1){
                oldTemp = newTemp;
                displayTemp(newTemp);
            }
            
            if(newTemp < 60){
                PORTA.OUT |= 0b00000010; //RELAY ON
            }
            else{
                PORTA.OUT &= 0b11111101; //RELAY OFF
            }
        
        }
        
        
        
    }       
}
