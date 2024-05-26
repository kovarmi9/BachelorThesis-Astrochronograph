#include "Tlacitko.h"

Tlacitko::Tlacitko(int pinCLK, int pinDT, int pinSW)
: _pinCLK(pinCLK), _pinDT(pinDT), _pinSW(pinSW), _poziceEnkod(0), _stavPred(LOW), _stavCLK(LOW), _stavSW(HIGH), _tlacitko_uvolneno(true)
{}

void Tlacitko::setup() {
  pinMode(_pinCLK, INPUT);
  pinMode(_pinDT, INPUT);
  pinMode(_pinSW, INPUT_PULLUP);
  _stavPred = digitalRead(_pinCLK);
}

void Tlacitko::loop() {
  _stavCLK = digitalRead(_pinCLK);
  if (_stavCLK != _stavPred) {
    if (digitalRead(_pinDT) != _stavCLK) {
      Serial.print("Rotace vpravo => | ");
      _poziceEnkod ++;
    } else {
      Serial.print("Rotace vlevo  <= | ");
      _poziceEnkod--;
    }
    Serial.print("Pozice enkoderu: ");
    Serial.println(_poziceEnkod);
  }
  _stavPred = _stavCLK;
  _stavSW = digitalRead(_pinSW);
  if (_stavSW == 0 && _tlacitko_uvolneno) {
    Serial.println("Stisknuto tlacitko enkoderu!");
    _tlacitko_uvolneno = false;
    delay(500);
  } else if (_stavSW == 1) {
    _tlacitko_uvolneno = true;
  }
}
