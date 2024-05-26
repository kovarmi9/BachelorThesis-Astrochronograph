#include "Bluetooth.h"

// definování objektu SoftwareSerial pro Bluetooth modul
SoftwareSerial bluetooth(TX, RX);

void handleBluetoothData(byte data) {
  switch (data) {
    case '0':
      // v případě přijetí znaku nuly vypneme LED diodu
      // a vypíšeme hlášku zpět do Bluetooth zařízení
      digitalWrite(pinLED, LOW);
      bluetooth.println("Vypni LED diodu.");
      break;
    case '1':
      // v případě přijetí jedničky zapneme LED diodu
      // a vypíšeme hlášku zpět do Bluetooth zařízení
      digitalWrite(pinLED, HIGH);
      bluetooth.println("Zapni LED diodu.");
      break;
    case 'a':
      // v případě přejetí znaku 'a' vypíšeme
      // čas od spuštění Arduina
      bluetooth.print("Cas od spusteni Arduina: ");
      bluetooth.print(millis() / 1000);
      bluetooth.println(" vterin.");
      break;
    case 'b':
      // zde je ukázka načtení většího počtu informací,
      // po přijetí znaku 'b' tedy postupně tiskneme 
      // další znaky poslané ve zprávě
      bluetooth.print("Nacitam zpravu: ");
      data = bluetooth.read();
      // v této smyčce zůstáváme do té doby,
      // dokud jsou nějaké znaky ve frontě
      while (bluetooth.available() > 0) {
        bluetooth.write(data);
        // krátká pauza mezi načítáním znaků
        delay(10);
        data = bluetooth.read();
      }
      bluetooth.println();
      break;
    case '\r':
      // přesun na začátek řádku - znak CR
      break;
    case '\n':
      // odřádkování - znak LF
      break;
    default:
      // v případě přijetí ostatních znaků
      // vytiskneme informaci o neznámé zprávě
      bluetooth.println("Neznamy prikaz.");
  }
}
