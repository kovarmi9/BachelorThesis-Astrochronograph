#include "Arduino.h" // C++ library for Wiring functionality
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
SoftwareSerial mySoftwareSerial(5, 6); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

void setup()
{
PlaySound();
}

void loop() {  
}

void PlaySound()
{
  mySoftwareSerial.begin(9600);
  if (!myDFPlayer.begin(mySoftwareSerial)) 
  {
    while (true);
  }
  myDFPlayer.volume(30);
  myDFPlayer.play(3);
}
