#include "SoftwareSerial.h"

#define RX 9
#define TX 10

// inicializace příkazu serial z knihovny SoftwareSerial
SoftwareSerial Stroj(TX, RX);

void setup() {
  Stroj.begin(9600);
  Serial.begin(9600);
}

void loop() {
  // posílání GET/M/WI21 do počítače každou vteřinu
  Stroj.println("GET/M/WI21");

  // čtení dat
  if (Stroj.available()) {
    String SerialData = Stroj.readStringUntil('\n');

    // vypisování dat na sériový monitor
    if (SerialData != "") {
      Serial.println(SerialData);
    }
  }

  delay(500);
}
