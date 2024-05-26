#include "Sound.h"
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

SoftwareSerial mySoftwareSerial(5, 6);
DFRobotDFPlayerMini myDFPlayer;

void PlaySoundSetup() {
  mySoftwareSerial.begin(9600);
  if (!myDFPlayer.begin(mySoftwareSerial)) {
    while (true);
  }
}

void PlaySound(int nazev, int volume) {
  myDFPlayer.volume(volume);
  myDFPlayer.play(nazev);
}
