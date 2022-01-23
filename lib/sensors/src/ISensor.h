#ifndef _ISensor_h
#define _ISensor_h

class ISensor{
    public:
      virtual float getValue(float temp);
      virtual float calibrate(float ecVal, float temp);
};

#endif