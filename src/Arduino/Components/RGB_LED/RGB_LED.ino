#include "RGB_LED.h"

void setup() {
  // nastavení pinů pro LED jako výstup
  pinMode(pinR, OUTPUT);
  pinMode(pinG, OUTPUT);
  pinMode(pinB, OUTPUT);
  // otestování všech barev pomocí podprogramu test
  test(pinR);
  delay(500);
  test(pinG);
  delay(500);
  test(pinB);
  delay(500);
}

void loop() {
  // pomocí tří smyček v sobě dojde k postupnému nastavení různých
  // kombinací barev z RGB spektra, vždy je maximální svítivost
  // rozdělena na 5 stupňů - celkem tedy 5^3 = 125 kombinací
  for (int red = 0; red < maxSvit; red += maxSvit / 5) {
    for (int green = 0; green < maxSvit; green += maxSvit / 5) {
      for (int blue = 0; blue < maxSvit; blue += maxSvit / 5) {
        // nastavení barvy na RGB LED modulu podle
        // aktuálních proměnných ve smyčkách
        nastavRGB(red, green, blue);
        // krátká pauza mezi barvami
        delay(200);
      }
    }
  }
}
