#ifndef RGB_LED_H
#define RGB_LED_H

#include <Arduino.h>

// nastavení vstupních pinů
#define pinR 9
#define pinG 10
#define pinB 6

// proměnná obsahující maximální svítivost LED diod
const int maxSvit = 255;

// deklarace funkcí
void test(int ledPin);
void nastavRGB(int cervena, int zelena, int modra);

#endif // RGB_LED_H
