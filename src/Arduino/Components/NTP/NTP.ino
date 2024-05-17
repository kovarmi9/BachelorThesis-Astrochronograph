// Libraries
#include <WiFiS3.h>
#include <WiFiUdp.h>

#include "arduino_secrets.h"

// WiFi Configuration
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
int status = WL_IDLE_STATUS;

// NTP Configuration
unsigned int localPort = 2390;
IPAddress timeServer(162, 159, 200, 123);
const int NTP_PACKET_SIZE = 48;
byte packetBuffer[NTP_PACKET_SIZE];
WiFiUDP Udp;

// Buffer for NTP time
char ntpTimeString[15] = "00:00:00.00"; // NTP Time

unsigned long ntpEpoch = 0; // NTP epoch time in seconds
unsigned long lastNtpUpdate = 0; // Last millis() when NTP was updated

void setup() {
  Serial.begin(9600); // Initialize Serial Monitor

  // Connect to WiFi
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");

  // Initialize UDP for NTP
  Udp.begin(localPort);

  // Get initial NTP time
  updateNTPTime();
}

void loop() {
  // Display and update time continuously
  displayNTPTime();
}

void updateNTPTime() {
  sendNTPpacket(timeServer);
  delay(1000); // Wait for a response

  if (Udp.parsePacket()) {
    Udp.read(packetBuffer, NTP_PACKET_SIZE);

    // Extract the 32 bit integer part of the timestamp
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    const unsigned long seventyYears = 2208988800UL;
    ntpEpoch = secsSince1900 - seventyYears;

    // Store the current millis when NTP was updated
    lastNtpUpdate = millis();
  }
}

void displayNTPTime() {
  unsigned long currentMillis = millis();
  unsigned long elapsedMillis = currentMillis - lastNtpUpdate;
  unsigned long currentEpoch = ntpEpoch + elapsedMillis / 1000;
  unsigned long centiseconds = (elapsedMillis % 1000) / 10;

  sprintf(ntpTimeString, "%02d:%02d:%02d.%02d", (currentEpoch % 86400L) / 3600, (currentEpoch % 3600) / 60, currentEpoch % 60, centiseconds);

  // Print the time
  Serial.print("NTP Time: ");
  Serial.println(ntpTimeString);
}

void sendNTPpacket(IPAddress& address) {
  memset(packetBuffer, 0, NTP_PACKET_SIZE);

  // Initialize values needed to form NTP request
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // Send NTP request
  Udp.beginPacket(address, 123); // NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}
