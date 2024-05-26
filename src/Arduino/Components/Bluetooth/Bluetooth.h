#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <Arduino.h>
#include <SoftwareSerial.h>

// nastavení propojovacích pinů Bluetooth a LED diody
#define RX A3
#define TX A2
#define pinLED 13

// deklarace objektu SoftwareSerial pro Bluetooth modul
extern SoftwareSerial bluetooth;

// deklarace funkcí
void handleBluetoothData(byte data);

#endif // BLUETOOTH_H
