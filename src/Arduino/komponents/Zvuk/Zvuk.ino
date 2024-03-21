#include "Arduino.h" // C++ library for Wiring functionality
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

// Create a SoftwareSerial instance for communication with the DFPlayer module
SoftwareSerial mySoftwareSerial(5, 6); // RX, TX

// Create a DFRobotDFPlayerMini instance
DFRobotDFPlayerMini myDFPlayer;

void setup(){
PlaySound(1,30); // Call the PlaySound function with argument 
}

void loop() {  
}

// Function to play a sound
void PlaySound(int nazev, int volume){
  // Initialization of the SoftwareSerial communication
  mySoftwareSerial.begin(9600);
  if (!myDFPlayer.begin(mySoftwareSerial)) 
  {
    while (true);
  }
  myDFPlayer.volume(volume); // Set the volume 0 - 30 
  myDFPlayer.play(nazev); // Play the mp3 file in the order corresponding to the order variable
  // zvuk byl vytvořen pomocí: https://ttsmaker.com/cs#google_vignette
}
