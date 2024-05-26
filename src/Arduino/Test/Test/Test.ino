// Libraries
#include <WiFiS3.h>
#include <WiFiUdp.h>
#include <DCF77.h>
#include <TimeLib.h>

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

// DCF77 Configuration
#define DCF_PIN 2
#define DCF_INTERRUPT 0
DCF77 DCF = DCF77(DCF_PIN, DCF_INTERRUPT);

// General Variables
unsigned long lastTime = 0;
char uptimeString[13]= "000:00:00.00"; // Uptime
char dcfTimeString[15] = "00:00:00.00"; // DCF77 Time
char ntpTimeString[15] = "00:00:00.00"; // NTP Time

void setup() {
  Serial.begin(9600); // Initialize Serial Monitor
  WiFi.begin(ssid, pass); // Connect to WiFi
  while (status != WL_CONNECTED) {
    status = WiFi.status();
    delay(1000);
  }
  Udp.begin(localPort); // Initialize UDP for NTP
  DCF.Start(); // Start DCF77
}

void loop() {
  updateUptime();
  updateDCFTime();
  updateNTPTime();
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
  delay(100); // Small delay to ensure packet is received
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

void displayTimes() {
  Serial.print("Uptime: ");
  Serial.print(uptimeString);
  Serial.print(" | DCF77: ");
  Serial.print(dcfTimeString);
  Serial.print(" | NTP: ");
  Serial.println(ntpTimeString);
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
  unsigned long lastTime = millis();
  while (true) {
    unsigned long currentTime = millis();
    unsigned long elapsedTime = currentTime - lastTime;
    if (elapsedTime >= 10) { // Update every 10 milliseconds
      unsigned int centiseconds = (millis() / 10) % 100;
      sprintf(timeString, "%02d:%02d:%02d.%02d", hour(), minute(), second(), centiseconds);
      lastTime = currentTime;
      break;
    }
  }
}
