#include "SDCard.h"
#include <SD.h>
#include "Sound.h"
#include "Button.h"

extern bool tlacitko_stisknuto;
extern char timeStr[12];
extern char dateString[12];
extern char cas_od_spusteni[13];
extern char posledni_ulozeny_cas[12];
extern char sd_string[4];
File myFile;

extern const int KARTA_CS_PIN;
extern const int BUTTON_PIN;

void SDCard_Init() {
  pinMode(KARTA_CS_PIN, INPUT_PULLUP);
  if (!SD.begin(KARTA_CS_PIN)) {
    strcpy(sd_string, "...");
  } else {
    strcpy(sd_string, " OK");
  }
}

void SDCard_Loop() {
  zmacknutitlacitka();
}

void zmacknutitlacitka() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    if (!tlacitko_stisknuto) {
      tlacitko_stisknuto = true;
      PlaySound(2, 30);
      casOdSpusteni();
      myFile = SD.open("ZAZNAM.txt", FILE_WRITE);
      if (myFile) {
        myFile.print(timeStr);
        myFile.print(" ");
        myFile.print(dateString);
        myFile.print("  ");
        myFile.println(cas_od_spusteni);
        myFile.close();
        strcpy(posledni_ulozeny_cas, timeStr);
      } else {
        while (1);
      }
    }
  } else {
    tlacitko_stisknuto = false;
  }
}
