#include "Button.h"
#include "Sound.h"
#include <Arduino.h>

extern bool tlacitko_stisknuto;
extern char cas_od_spusteni[13];

extern const int BUTTON_PIN;

void Button_Init() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void Button_Check() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    if (!tlacitko_stisknuto) {
      tlacitko_stisknuto = true;
      PlaySound(2, 30);
      casOdSpusteni();
      // Further code here
    }
  } else {
    tlacitko_stisknuto = false;
  }
}

void casOdSpusteni() {
  unsigned long cas = millis();
  unsigned int hodiny = cas / 3600000;
  unsigned int minuty = (cas / 60000) % 60;
  unsigned int sekundy = (cas / 1000) % 60;
  unsigned int setiny_sekundy = (cas / 10) % 100;
  sprintf(cas_od_spusteni, "%03d:%02d:%02d.%02d", hodiny, minuty, sekundy, setiny_sekundy);
}
