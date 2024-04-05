#ifndef RT_H
#define RT_H

int getTemperatureFromOnChipTempSensor(u32 u32_adcReading);
int getTemperatureFromThermistor(u32 u32_adcReading);
void testTemperature(u32 u32_tempK);

#endif
