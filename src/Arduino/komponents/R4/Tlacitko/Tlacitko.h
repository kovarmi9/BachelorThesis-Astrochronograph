#ifndef TLACITKO_H
#define TLACITKO_H

#include "Arduino.h"

class Tlacitko {
  public:
    Tlacitko(int pinCLK, int pinDT, int pinSW);
    void setup();
    void loop();
  private:
    int _pinCLK;
    int _pinDT;
    int _pinSW;
    int _poziceEnkod;
    int _stavPred;
    int _stavCLK;
    int _stavSW;
    bool _tlacitko_uvolneno;
};

#endif
