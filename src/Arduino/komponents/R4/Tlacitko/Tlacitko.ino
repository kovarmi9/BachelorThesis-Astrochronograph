#include "Tlacitko.h"

Tlacitko tlacitko(3, 4, 5);

void setup() {
  Serial.begin(9600);
  tlacitko.setup();
}

void loop() {
  tlacitko.loop();
}
