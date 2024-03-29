/*
 * File:   rt.c
 * Author: heinz
 *
 * Created on February 24, 2024, 12:27 PM
 */

#include <avr/io.h>
#include <stdint.h>
#include <math.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

int getTemperatureFromOnChipTempSensor(u32 u32_adcReading);
int getTemperatureFromThermistor(u32 u32_adcReading);
void testTemperature(u32 u32_tempK);


int main(void) {
    /* Replace with your application code */
            
        //Using on-chip temperature sensor
        
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
              
        //Set up LED as Output Pins for testing
        PORTA.DIRSET = 0b00000011;
        
        PORTA.OUT &= ~(0x01);

        
    while (1) {

        if(ADC0.INTFLAGS & 0x01){ //Result is ready from ADC
            u32 u32_adcReading = ADC0.RES;
            u32 u32_tempC;
            ADC0.INTFLAGS &= 0x01; //Clear flag to read next time
            
            //Implement thermistor reading
            u32_tempC = getTemperatureFromThermistor(u32_adcReading);
            
            if(u32_tempC < 60){
                PORTA.OUT |= 0b00000010;
            }
            else{
                PORTA.OUT &= 0b11111101;
            }
    
        }
        
    }
}

int getTemperatureFromOnChipTempSensor(u32 u32_adcReading){
    
//    ACTUAL   ||  MEASURED
//    19 C         13.85 C
    
    u16 u16_offset = SIGROW.TEMPSENSE1 * 2.048/5.0;
    u16 u16_slope = SIGROW.TEMPSENSE0 *  5.0/2.048;
                
    u32 u32_tempK = u16_offset - u32_adcReading;
    u32_tempK *= u16_slope;
    u32_tempK += 0x0800;
    u32_tempK >>= 12; //Temp 1 Kelvin precision
    return u32_tempK - 273;
}


/*
@fn int getTemperatureFromThermistor(u32 u32_adcReading)
@brief Calculates and returns temp in degree C from ADC reading

REQUIRES:
-Valid u32 adcReading and constants in function match voltage divider and
 thermistor 
 
PROMISES:
-Returns temperature in degrees celsius
*/

int getTemperatureFromThermistor(u32 u32_adcReading){
//    ACTUAL   ||  MEASURED
//    21 C         20 C/ 293 K
     const float R_O = 10000;
     const float B = 3270;
     const  T_O = 298;
     const int R_fixed = 22000;
     
     float VO = u32_adcReading*5.0/4096;
     float Rt = (VO*R_fixed)/(5-VO);
     int u32_tempK = pow(
                    (1.0/T_O) + (1.0/B)*log((Rt/R_O)),
                     -1);
     
     return u32_tempK - 273;
}

/*
@fn  void testTemperature(u32 u32_tempK)
@brief Turns on LED if temperature is between the lowerbound and higher bound
 * of temperature 

REQUIRES:
-NONE
 
PROMISES:
-Turns on LED if temperature is between the bounds else LED is off
*/
void testTemperature(u32 u32_tempK){
    int lowerBound = 20;
    int higherBound = 40;
    if(u32_tempK > lowerBound && u32_tempK < higherBound){
        PORTA.OUT |= 0x01;
    }
    else{
        PORTA.OUT &= ~(0x01);
    }
}

