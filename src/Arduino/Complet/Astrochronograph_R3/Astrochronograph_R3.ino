/* This code initializes the internal time of the Arduino with the time obtained from DCF77, connects the SD card, 
 * and upon pressing a button, creates a file "Zaznam.txt" on it, in which it saves the current time obtained from DCF77
 * in the format HH:MM:SS.SS DD MM YYYY (00:00:00.00 01 01 1970) and the time since the Arduino started in the format 
 * HHH:MM:SS.SS (000:00:00.00). This code also displays the current time, the last recorded time, the status of the SD card 
 * (SD:... / SD: OK), and the status of the time initialization from DCF77 (DCF:... / DCF: OK) on the display.
 *
 * THIS CODE WAS CREATED FOR ARDUINO UNO!!!
 *
 * Wiring:
 *
 * SD card slot:
 * MOSI - pin 11
 * MISO - pin 12
 * SCK  - pin 13
 * CS   - pin 4
 * Button (rotary encoder):
 * SW   - pin 8
 * Display:
 * SCK  - pin A5
 * SDA  - pin A4
 * DCF77 receiver:
 * TCO  - pin 2
 *
 * Description of the DCF77 receiver: https://www.pollin.de/p/dcf-77-empfangsmodul-dcf1-810054
 * Description of the display: https://dratek.cz/arduino/1479-iic-i2c-displej-oled-0.91-128x32-bily-3-3-v-5v-pro-iot-arduino-raspbery.html
 * Description of the SD card: https://www.laskakit.cz/microsd-card-modul-spi/?gclid=EAIaIQobChMI4dXtueG2_QIVR-J3Ch1mqwbHEAQYASABEgI9BPD_BwE#relatedFiles
 * https://github.com/arduino-libraries/SD/tree/master/examples
 * Description of the button (rotary encoder): https://dratek.cz/arduino/837-rotacni-enkoder.html
 */

/* CURRENT PROBLEMS!!!
 * When the SD card is disconnected, the display output is not smooth.
 * If the time is updated while the SD card is disconnected, the whole program freezes.
 * 90% memory usage on Arduino Uno is quite a lot... needs optimization.
 * Functions that are called only once in the code should be converted to inline functions to save space.
 * The code could use some more organization.
 */

// Define pins
#define DCF_PIN 2 // Connection pin to DCF 77 device
#define DCF_INTERRUPT 0 // Interrupt number associated with pin
#define BUTTON_PIN 8 // Define the pin for the button SW
#define KARTA_CS_PIN 4 // Define the pin for the SD card CS

// Libraries
#include "Arduino.h" // C++ library for Wiring functionality
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include <DCF77.h> // Library for connecting to DCF 77
#include <TimeLib.h> // Library for working with time
#include <SD.h> // Library for working with SD card
#include <U8glib.h> // Library for working with OLED display

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

time_t time;
DCF77 DCF = DCF77(DCF_PIN, DCF_INTERRUPT);

// Variables for OLED display
U8GLIB_SSD1306_128X64 mujOled(U8G_I2C_OPT_NONE); // Object for OLED display
char timeStr[12] = "00:00:00.00"; // Variable for storing time in format HH:MM:SS:SS
char sd_string[4] = "..."; // Variable for storing SD card status
char cas_string[4] = "..."; // Variable for storing DCF77 signal status

File myFile; // Object for working with files on the SD card

bool tlacitko_stisknuto = false; // Button status

char cas_od_spusteni[13] = "000:00:00.00"; // Predefined string with time since start
char posledni_ulozeny_cas[12] = "           ";

void setup() {
  // Initialization of the SoftwareSerial communication
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
  } else {
    strcpy(sd_string, " OK");
  }
}

void zobrazitNaDisplay(const char* cas, const char* sd, const char* dcf, const char* zmereny_cas) {
  /* Used fonts:
   * Fonts can be changed according to: https://github.com/olikraus/u8glib/wiki/fontsize
   * Adobe X11 Font
   * u8g_font_courB14
   * u8g_font_courR08
   */
  mujOled.firstPage();
  do {
    mujOled.setFont(u8g_font_courB14);
    mujOled.drawStr(0, 32, cas);
    mujOled.setFont(u8g_font_courR08);
    mujOled.drawStr(0, 52, zmereny_cas);
    mujOled.setFont(u8g_font_courR08);
    mujOled.drawStr(92, 44, "SD:");
    mujOled.drawStr(110, 44, sd);
    mujOled.drawStr(86, 52, "DCF:");
    mujOled.drawStr(110, 52, dcf);
  } while (mujOled.nextPage());
}

void zmacknutitlacitka() {
  // Function to detect button press and execute commands after pressing it
  if (digitalRead(BUTTON_PIN) == LOW) {
    if (!tlacitko_stisknuto) {
      tlacitko_stisknuto = true;
      PlaySound(2, 30); // Call the PlaySound function with argument 
      casOdSpusteni(); // Update cas_od_spusteni
      myFile = SD.open("ZAZNAM.txt", FILE_WRITE);
      if (myFile) {
        // Commands executed after pressing the button
        myFile.print(timeStr); // Save to ZAZNAM.txt time obtained from DCF77 in format (HH:MM:SS.SS)
        myFile.print(" "); // Add space
        myFile.print(dateString); // Save date in format (DD MM YYYY)
        myFile.print("  "); // Add two spaces
        myFile.println(cas_od_spusteni);
        myFile.close();
        strcpy(posledni_ulozeny_cas, timeStr); // Assign the value of one array to another array
        // End of commands executed after pressing the button
      } else {
        while (1);
      }
    }
  } else {
    tlacitko_stisknuto = false; // Ensures that one button press is considered only as one press
  }
}

void casOdSpusteni() {
  // Function that saves the time since start to the variable "cas_od_spusteni"
  // It is called in the function "void zmacknutitlacitka()"
  unsigned long cas = millis();
  // Splitting the time since start into hours, minutes, seconds, and hundredths
  unsigned int hodiny = cas / 3600000;
  unsigned int minuty = (cas / 60000) % 60;
  unsigned int sekundy = (cas / 1000) % 60;
  unsigned int setiny_sekundy = (cas / 10) % 100;
  // Formatting and saving the time since start to "cas_od_spusteni"
  sprintf(cas_od_spusteni, "%03d:%02d:%02d.%02d", hodiny, minuty, sekundy, setiny_sekundy);
}

void tvorbaStringu() {
  // Function to create strings with time and date obtained from DCF77
  sprintf(timeString, "%02d:%02d:%02d", hour(), minute(), second());
  sprintf(dateString, "%02d %02d %04d", day(), month(), year());
}

void dcfCas() {
  // Function to update the internal time of the Arduino with DCF77
  time_t DCFtime = DCF.getTime();
  if (DCFtime != 0) {
    setTime(DCFtime);
    sprintf(cas_string, " OK");
    tvorbaStringu();
    PlaySound(1, 30); // Call the PlaySound function with argument 
    myFile = SD.open("TEST.txt", FILE_WRITE); // Open file to write to
    if (myFile) {
      myFile.print("Time last updated: "); // Write to file TEST.txt
      myFile.print(timeString); // Save to Cas_aktualizace.txt time obtained from DCF77 in format (HH:MM:SS.SS)
      myFile.print(" ");
      myFile.println(dateString);
      myFile.close(); // Close file
    } else {
      while (1);
    }
  }
}

void setiny() {
  // Function to create hundredths of seconds
  if (strcmp(timeString, previousTimeString) != 0) { // Compare time strings
    lastTime = millis();
    strcpy(previousTimeString, timeString); // Assign new time string to the previous time string
  }
  float elapsedTime = (float)(millis() - lastTime) / 1000; // Calculate the time since the strings differed
  snprintf(milliString, 5, "%02d", (int)(elapsedTime * 100) % 100); // Create a string with hundredths of seconds
  sprintf(timeStr, "%s.%s", timeString, milliString); // Join the time string and the string with hundredths of seconds
}

// Function to play a sound
void PlaySound(int nazev, int volume) {
  myDFPlayer.volume(volume); // Set the volume 0 - 30 
  myDFPlayer.play(nazev); // Play the mp3 file in the order corresponding to the order variable
  // The sound was created using: https://ttsmaker.com/cs#google_vignette
}

void PlaySoundSetup() {
  mySoftwareSerial.begin(9600);
  if (!myDFPlayer.begin(mySoftwareSerial)) {
    while (true);
  }
}
