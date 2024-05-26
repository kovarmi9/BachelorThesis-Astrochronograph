#include "RGB_LED.h"

void test(int ledPin) {
  // nastavení intenzity na daný pin vzestupně od 0 do hodnoty maxSvit
  for (int intenzita = 0; intenzita <= maxSvit; intenzita += 5) {
    // řízení jasu pomocí PWM na daném pinu
    analogWrite(ledPin, intenzita);
    // krátké zpoždění pro lepší efekt
    delay(30);
  }
  // nastavení intenzity na daný pin sestupně od hodnoty maxSvit do 0
  for (int intenzita = maxSvit; intenzita >= 0; intenzita -= 5) {
    // řízení jasu pomocí PWM na daném pinu
    analogWrite(ledPin, intenzita);
    // krátké zpoždění pro lepší efekt
    delay(30);
  }
}

void nastavRGB(int cervena, int zelena, int modra) {
  // nastavení všech barev na zvolené intenzity
  analogWrite(pinR, cervena);
  analogWrite(pinG, zelena);
  analogWrite(pinB, modra);
}
