#include "AnalogEC.h"
#include "Particle.h"

AnalogEC::AnalogEC(int pin){
    _pin = pin;
    begin();
}

void AnalogEC::begin(){
    pinMode(_pin, INPUT);
}

float AnalogEC::getValue(float temp){
    EC_Vo = (analogRead(_pin)*3.3)/4096.0;
    EC_val_no_temp = (EC_Vo*1.386)/0.345;
    EC_val_with_temp = calibrate(EC_val_no_temp, temp);

    return EC_val_with_temp;
}

float AnalogEC::calibrate(float ecVal, float temp){
    Compensation_Coefficient = 1.0+0.02*(25.0-temp);
    temperature_compensation = ecVal / Compensation_Coefficient;     //temperature compensation

    return temperature_compensation;
}