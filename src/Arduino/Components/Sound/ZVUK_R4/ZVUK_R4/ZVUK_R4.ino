#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
SoftwareSerial mySoftwareSerial(A0, A1); // TX, RX
//SoftwareSerial mySoftwareSerial(D3, D1); // TX, RX
DFRobotDFPlayerMini myDFPlayer;

void setup(){
  PlaySoundSetup();
}

void loop() {
  static unsigned long timer = millis();
    if (millis() - timer > 3000) {
    timer = millis();
    myDFPlayer.volume(30); //Set volume value. From 0 to 30
    myDFPlayer.play(001); //Play first mp3 every 3 second.
  }
}

void PlaySoundSetup(){
  mySoftwareSerial.begin(9600);
  if (!myDFPlayer.begin(mySoftwareSerial)) 
  {
    while (true);
  }
  myDFPlayer.volume(10); //Set volume value. From 0 to 30
  myDFPlayer.play(3); //Play the first mp3
}