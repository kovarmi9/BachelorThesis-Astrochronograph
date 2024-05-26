#define DCF_PIN 2
#define DCF_INTERRUPT 0
#define KARTA_CS_PIN 4
#define BUTTON_PIN 8


#include "DCF.h"
#include <DCF77.h>
#include <TimeLib.h>
#include "Sound.h"
#include <SD.h>

extern char cas_string[4];
extern char timeStr[12];
extern char dateString[12];
extern File myFile;

extern const int DCF_PIN;
extern const int DCF_INTERRUPT;

DCF77 DCF = DCF77(DCF_PIN, DCF_INTERRUPT);

// Declare tvorbaStringu here
void tvorbaStringu();

void DCF_Start() {
  DCF.Start();
}

void DCF_Update() {
  time_t DCFtime = DCF.getTime();
  if (DCFtime != 0) {
    setTime(DCFtime);
    sprintf(cas_string, " OK");
    // Call tvorbaStringu here
    tvorbaStringu();
    PlaySound(1, 30);
    myFile = SD.open("TEST.txt", FILE_WRITE);
    if (myFile) {
      myFile.print("cas naposledy aktualizovan: ");
      myFile.print(timeStr);
      myFile.print(" ");
      myFile.println(dateString);
      myFile.close();
    } else {
      while (1);
    }
  }
}

void tvorbaStringu() {
  sprintf(timeStr, "%02d:%02d:%02d", hour(), minute(), second());
  sprintf(dateString, "%02d %02d %04d", day(), month(), year());
}
