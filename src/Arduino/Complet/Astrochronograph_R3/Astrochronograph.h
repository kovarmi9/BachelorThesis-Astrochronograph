#ifndef ASTROCHRONOGRAPH_H
#define ASTROCHRONOGRAPH_H

#include <Arduino.h>
#include <DCF77.h>
#include <TimeLib.h>
#include <SD.h>
#include <U8glib.h>

// Pin definitions
#define DCF_PIN 2
#define DCF_INTERRUPT 0
#define BUTTON_PIN 8
#define KARTA_CS_PIN 4

// Function declarations
void tvorbaStringu();
void dcfCas();
void setiny();
void zmacknutitlacitka();
void casOdSpusteni();

extern char timeStr[12];
extern char sd_string[4];
extern char cas_string[4];
extern char posledni_ulozeny_cas[12];

#endif // ASTROCHRONOGRAPH_H
