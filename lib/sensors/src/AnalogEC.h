#ifndef _AnalogEC_h
#define _AnalogEC_h

#include "ISensor.h"

class AnalogEC: public ISensor{

    public:
    AnalogEC(int pin);
    void begin();
    float getValue(float temp) override;
    float calibrate(float ecVal, float temp) override;

    private:

    int _pin;
    float EC_Vo;
    float EC_val_no_temp;
    float EC_val_with_temp;
    float Compensation_Coefficient;
    float temperature_compensation;


};

#endif