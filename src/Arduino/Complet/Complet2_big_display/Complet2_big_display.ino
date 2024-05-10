/*
 * This code initializes Arduino's internal time with time obtained from DCF77, 
 * connects the SD card, and upon pressing the button, creates a file Zaznam.txt 
 * to which it saves the current time obtained from DCF77 in the format 
 * HH:MM:SS.SS DD MM year (00:00:00.00 01 01 1970) and the time since the Arduino 
 * was started in the format HHH:MM:SS.SS (000:00:00.00). This code also displays 
 * the current time, the last recorded time, the status of the SD card (SD:... / SD: OK), 
 * and the status of initializing the time from DCF77 (DCF:... / DCF: OK).
 *
 * THIS CODE WAS CREATED FOR ARDUINO UNO R3!!!
 *
 * Where to connect what?
 *
 * SD card slot:
 * MOSI - pin 11
 * MISO - pin 12
 * SCK  - pin 13
 * CS   - pin  4
 * Button (rotary encoder):
 * SW   - pin  8
 * Display:
 * SCK  - pin A5
 * SDA  - pin A4
 * DCF77 receiver:
 * TCO  - pin  2
 *
 *
 * Description of DCF77 receiver: https://www.pollin.de/p/dcf-77-empfangsmodul-dcf1-810054
 * Description of display: https://dratek.cz/arduino/1479-iic-i2c-displej-oled-0.91-128x32-bily-3-3-v-5v-pro-iot-arduino-raspbery.html
 * Description of SD card: https://www.laskakit.cz/microsd-card-modul-spi/?gclid=EAIaIQobChMI4dXtueG2_QIVR-J3Ch1mqwbHEAQYASABEgI9BPD_BwE#relatedFiles
 * https://github.com/arduino-libraries/SD/tree/master/examples
 * Description of button (rotary encoder): https://dratek.cz/arduino/837-rotacni-enkoder.html
 *
 */


// Pin definitions
#define DCF_PIN 2 // Connection pin to DCF 77 device
#define DCF_INTERRUPT 0 // Interrupt number associated with pin
#define BUTTON_PIN 8 // Defines the pin for the button's SW
#define KARTA_CS_PIN 4 // Defines the pin for the CS of the SD card

// Libraries
#include "Arduino.h" // C++ library for Wiring functionality
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include <DCF77.h> // Library for connecting to DCF 77
#include <TimeLib.h>// Library for working with time
#include <SD.h> // Library for working with SD card
#include <U8g2lib.h> // Library for working with OLED display

// Variables for DCF 77
unsigned long lastTime = 0;
char milliString[4] = ".00";
char timeString[9] = "00:00:00";
char dateString[12] = "00 00 1970";
char previousTimeString[9] = "00:00:00";

// Create a SoftwareSerial instance for communication with the DFPlayer module
SoftwareSerial mySoftwareSerial(5, 6); // RX, TX

// Create a DFRobotDFPlayerMini instance
DFRobotDFPlayerMini myDFPlayer;

time_t Time;
DCF77 DCF = DCF77(DCF_PIN,DCF_INTERRUPT);

// Variables for OLED display
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE); // Software I2C // Object for OLED display
char timeStr[12] = "00:00:00.00"; // Variable for storing time in the format HH:MM:SS:SS
char sd_string[4] = "..."; // Variable for storing the status of the SD card
char cas_string[4] = "..."; // Variable for storing the status of the DCF77 signal

File myFile; // Object for working with a file on the SD card

bool tlacitko_stisknuto = false; // Button state

char cas_od_spusteni[13]= "000:00:00.00"; // Predefined string with time since startup
char posledni_ulozeny_cas[12] ="           ";

void setup() {
  u8g2.begin();
  PlaySoundSetup();
  DCF.Start();
  pinMode(KARTA_CS_PIN, INPUT_PULLUP);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  kartaLoop();
  dcfCas();
  tvorbaStringu();
  setiny();
  zobrazitNaDisplay(timeStr, sd_string, cas_string, posledni_ulozeny_cas);
  zmacknutitlacitka();
}

void kartaLoop() {
  // Function to check the status of the SD card
  if (!SD.begin(KARTA_CS_PIN)) {
    strcpy(sd_string, "...");
  } 
  else {
    strcpy(sd_string, " OK");
  }
}


void zobrazitNaDisplay(const char* cas, const char* sd, const char* dcf,const char* zmereny_cas) {
/* 
 * Fonts can be changed according to: https://github.com/olikraus/u8g2/wiki
 */
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_9x18_tr);
    u8g2.drawStr(5, 15, timeStr);
    u8g2.setFont(u8g2_font_6x12_tr);
    //u8g2.drawStr(5, 60, zmereny_cas);
    u8g2.drawStr(92, 30, "SD:");
    u8g2.drawStr(110, 30, sd_string);
    u8g2.drawStr(86, 40, "DCF:");
    u8g2.drawStr(110, 40, cas_string);
  } while (u8g2.nextPage());
}


void zmacknutitlacitka(){
  // Function to recognize button press and execute commands after pressing
  if (digitalRead(BUTTON_PIN) == LOW) {
    if (!tlacitko_stisknuto) {
      tlacitko_stisknuto = true;
      PlaySound(2,30); // Call the PlaySound function with argument 
      casOdSpusteni(); // Update cas_od_spusteni
      myFile = SD.open("ZAZNAM.txt", FILE_WRITE);
      if (myFile) {
        // Commands executed after button press
        myFile.print(timeStr); // Save time obtained from DCF77 to ZAZANAM.txt in format (HH:MM:SS.SS)
        myFile.print(" "); // Add space
        myFile.print(dateString); // Save date in format (DD MM year)
        myFile.print("  "); // Add two spaces
        myFile.println(cas_od_spusteni);
        myFile.close();
        strcpy(posledni_ulozeny_cas, timeStr);// Copy value from one array to another
        // End of commands executed after button press
        } else {
        while (1);
      }
    }
  } else {
    tlacitko_stisknuto = false; // Button press is only considered as one press
  }
}

void casOdSpusteni() {
  // Function that saves the time since startup in the "cas_od_spusteni" variable
  // Called in the "void zmacknutitlacitka()" function
  unsigned long cas = millis();
  // Splitting the time since startup into hours, minutes, seconds, and hundredths
  unsigned int hodiny = cas / 3600000;
  unsigned int minuty = (cas / 60000) % 60;
  unsigned int sekundy = (cas / 1000) % 60;
  unsigned int setiny_sekundy = (cas / 10) % 100;
  // Formatting and saving the time since startup to "cas_od_spusteni"
  sprintf(cas_od_spusteni, "%03d:%02d:%02d.%02d", hodiny, minuty, sekundy, setiny_sekundy);
}


void tvorbaStringu(){
  // Function for creating strings with time and date obtained from DCF77
  sprintf(timeString, "%02d:%02d:%02d", hour(), minute(), second());
  sprintf(dateString, "%02d %02d %04d", day(), month(), year());
}

void dcfCas(){
  // Function for updating Arduino's internal time with DCF77
  time_t DCFtime = DCF.getTime();
  if (DCFtime != 0) {
    setTime(DCFtime);
    sprintf(cas_string, " OK");
    tvorbaStringu();
    PlaySound(1,30); // Call the PlaySound function with argument 
    myFile = SD.open("TEST.txt", FILE_WRITE); // Open file to write to
    if (myFile) {
      myFile.print("cas naposledy aktualizovan: "); // Write to the Cas_aktualizace.txt file
      myFile.print(timeString); // Save time obtained from DCF77 in format (HH:MM:SS.SS)
      myFile.print(" ");
      myFile.println(dateString);
      myFile.close(); // Close file
    } else {
    while (1);
    }
  }
}

void setiny() {
  // Function for creating hundredths of a second
  if (strcmp(timeString, previousTimeString) != 0) { // Compare time strings
    lastTime = millis();
    strcpy(previousTimeString, timeString);// Assign new time string to previous time string
  }
  float elapsedTime = (float)(millis() - lastTime) / 1000; // Calculate time since the strings differed
  snprintf(milliString, 5, "%02d", (int)(elapsedTime * 100) % 100); // Create string with milliseconds
  sprintf(timeStr, "%s.%s", timeString, milliString); // time string + milliseconds string
}

// Function to play a sound
void PlaySound(int nazev, int volume){
  myDFPlayer.volume(volume); // Set the volume 0 - 30 
  myDFPlayer.play(nazev); // Play the mp3 file in the order corresponding to the order variable
  // Sound was created using: https://ttsmaker.com/cs#google_vignette
}

void PlaySoundSetup(){
  mySoftwareSerial.begin(9600);
  if (!myDFPlayer.begin(mySoftwareSerial)) 
  {
    while (true);
  }
}