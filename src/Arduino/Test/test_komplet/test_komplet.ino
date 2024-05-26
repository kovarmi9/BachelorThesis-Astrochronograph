#include <WiFiS3.h>
#include <WiFiUdp.h>
#include <DCF77.h>
#include <TimeLib.h>
#include <TinyGPSPlus.h> // Přidání knihovny TinyGPS++

// Pro WiFi konfiguraci
#include "arduino_secrets.h"

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
int status = WL_IDLE_STATUS;

// NTP Konfigurace
unsigned int localPort = 2390;
IPAddress timeServer(162, 159, 200, 123);
const int NTP_PACKET_SIZE = 48;
byte packetBuffer[NTP_PACKET_SIZE];
WiFiUDP Udp;

// DCF77 Konfigurace
#define DCF_PIN 2
#define DCF_INTERRUPT 0
DCF77 DCF = DCF77(DCF_PIN, DCF_INTERRUPT);

// GPS Konfigurace
TinyGPSPlus gps;

// Obecné proměnné
unsigned long startTime = 0;
char uptimeString[13] = "000:00:00.00";
char dcfTimeString[15] = "00:00:00.00";
char ntpTimeString[15] = "00:00:00.00";
char gpsTimeString[15] = "00:00:00.00";
char milliString[5];
char previousTimeString[15] = "00:00:00.00";

void setup() {
  Serial.begin(9600); // Inicializace sériového monitoru
  Serial1.begin(9600); // Připojení GPS modulu na Serial1
  WiFi.begin(ssid, pass); // Připojení k WiFi
  while (status != WL_CONNECTED) {
    status = WiFi.status();
    delay(1000);
  }
  Udp.begin(localPort); // Inicializace UDP pro NTP
  DCF.Start(); // Start DCF77
  startTime = millis(); // Nastavení času spuštění
}

void loop() {
  updateUptime();
  updateDCFTime();
  updateNTPTime();
  updateGPSTime();
  displayTimes();
}

void updateUptime() {
  unsigned long millisec = millis();
  unsigned int hours = millisec / 3600000;
  unsigned int minutes = (millisec / 60000) % 60;
  unsigned int seconds = (millisec / 1000) % 60;
  unsigned int centiseconds = (millisec / 10) % 100;
  sprintf(uptimeString, "%03d:%02d:%02d.%02d", hours, minutes, seconds, centiseconds);
}

void updateDCFTime() {
  time_t DCFtime = DCF.getTime();
  if (DCFtime != 0) {
    setTime(DCFtime);
    setiny(dcfTimeString);
  }
}

void updateNTPTime() {
  sendNTPpacket(timeServer);
  delay(100); // Malé zpoždění pro zajištění přijetí paketu
  if (Udp.parsePacket()) {
    Udp.read(packetBuffer, NTP_PACKET_SIZE);
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    const unsigned long seventyYears = 2208988800UL;
    unsigned long epoch = secsSince1900 - seventyYears;
    unsigned long millisec = (millis() % 1000) / 10;
    sprintf(ntpTimeString, "%02d:%02d:%02d.%02d", (epoch % 86400L) / 3600, (epoch % 3600) / 60, epoch % 60, millisec);
  }
}

void updateGPSTime() {
  while (Serial1.available() > 0) {
    gps.encode(Serial1.read());
  }
  
  if (gps.time.isValid()) {
    sprintf(gpsTimeString, "%02d:%02d:%02d.%02d", gps.time.hour(), gps.time.minute(), gps.time.second(), gps.time.centisecond());
    setiny(gpsTimeString);
  }
}

void displayTimes() {
  Serial.print("Uptime: ");
  Serial.print(uptimeString);
  Serial.print(" | DCF77: ");
  Serial.print(dcfTimeString);
  Serial.print(" | NTP: ");
  Serial.print(ntpTimeString);
  Serial.print(" | GPS: ");
  Serial.println(gpsTimeString);
}

void sendNTPpacket(IPAddress& address) {
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  packetBuffer[0] = 0b11100011;
  packetBuffer[1] = 0;
  packetBuffer[2] = 6;
  packetBuffer[3] = 0xEC;
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 0;
  Udp.beginPacket(address, 123);
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

void setiny(char* timeString) {
  static unsigned long lastTime = 0;
  if (strcmp(timeString, previousTimeString) != 0) {
    lastTime = millis();
    strcpy(previousTimeString, timeString);
  }
  float elapsedTime = (float)(millis() - lastTime) / 1000;
  snprintf(milliString, 5, "%02d", (int)(elapsedTime * 100) % 100);
}
