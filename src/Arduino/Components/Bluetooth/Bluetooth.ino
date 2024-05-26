#include "Bluetooth.h"

void setup() {
  // zahájení komunikace s Bluetooth modulem
  // skrze Softwarovou sériovou linku rychlostí 9600 baud
  bluetooth.begin(9600);
  bluetooth.println("Arduino zapnuto, test Bluetooth..");
  // nastavení pinu s LED diodou jako výstup
  pinMode(pinLED, OUTPUT);
}

void loop() {
  // proměnná pro ukládání dat z Bluetooth modulu
  byte BluetoothData;
  // kontrola Bluetooth komunikace, pokud je dostupná nová
  // zpráva, tak nám tato funkce vrátí počet jejích znaků
  if (bluetooth.available() > 0) {
    // načtení prvního znaku ve frontě do proměnné
    BluetoothData = bluetooth.read();
    // dekódování přijatého znaku
    handleBluetoothData(BluetoothData);
  }
  // krátká pauza mezi kontrolami komunikace Bluetooth modulu
  delay(100);
}
