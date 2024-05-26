#include "Sound.h"
#include <Arduino.h>

// Create a SoftwareSerial instance for communication with the DFPlayer module
SoftwareSerial mySoftwareSerial(5, 6); // RX, TX

// Create a DFRobotDFPlayerMini instance
DFRobotDFPlayerMini myDFPlayer;

void PlaySoundSetup(){
  mySoftwareSerial.begin(9600);
  if (!myDFPlayer.begin(mySoftwareSerial)) 
  {
    while (true);
  }
}

void PlaySound(int nazev, int volume){
  myDFPlayer.volume(volume); // Set the volume 0 - 30 
  myDFPlayer.play(nazev); // Play the mp3 file in the order corresponding to the order variable
  // zvuk byl vytvořen pomocí: https://ttsmaker.com/cs#google_vignette
}
