/*
Modul co mám já má prohorezný RX TX tak je potřeba prohodit kabely pokud to nefunguje
*/

#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
SoftwareSerial mySoftwareSerial(A5, A4); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

void setup(){
  PlaySoundSetup();
}

void loop() {
  static unsigned long timer = millis();
    if (millis() - timer > 3000) {
    timer = millis();
    myDFPlayer.volume(30); //Set volume value. From 0 to 30
    myDFPlayer.play(2); //Play first mp3 every 3 second.
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