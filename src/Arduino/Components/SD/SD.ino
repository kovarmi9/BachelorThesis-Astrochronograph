#define KARTA_CS_PIN 4

#include <SD.h>

File myFile;

void setup() {
  Serial.begin(9600);
  pinMode(KARTA_CS_PIN, INPUT_PULLUP);
}

void loop() {
  SDCheck();
}

void SDCheck(){
  if (!SD.begin(KARTA_CS_PIN)) {
    Serial.println("SD karta není připojena");
    return;
  }
  Serial.println("SD karta je připojena");
  SDWrite();
}

void SDWrite(){
  myFile = SD.open("FileName.txt", FILE_WRITE);
  if (myFile) {
    myFile.print("text");
    myFile.close();
    } else {
      while (1);
  }

}
