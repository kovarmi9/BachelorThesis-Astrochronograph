#include "Tlacitko.h"

Tlacitko tlacitko(7, 5, 4);

void setup() {
  Serial.begin(9600);
  tlacitko.setup();
}

void loop() {
  tlacitko.loop();
}
