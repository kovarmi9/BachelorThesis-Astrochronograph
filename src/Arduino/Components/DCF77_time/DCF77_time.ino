/*
 * DCF77.ino
 * This code synchronized time from a DCF77 receiver
 * This code was tested on https://www.pollin.de/p/dcf-77-empfangsmodul-dcf1-810054
 * This code is based on https://github.com/thijse/Arduino-Libraries/downloads and https://github.com/ThoMo/DCF77

*/

// Libraries
#include "Arduino.h" // C++ library for Wiring functionality
#include <DCF77.h> // Library for connecting to DCF 77: https://github.com/thijse/Arduino-Libraries/downloads
#include <TimeLib.h> // Library for working with time: http://www.arduino.cc/playground/Code/Time


// Define DCF pin and interrupt number
#define DCF_PIN 2 // Connection pin to DCF 77 device
#define DCF_INTERRUPT 0 // Interrupt number associated with pin

// Variables for DCF77

// Variable for creating miliseconds 
unsigned long lastTime = 0;

char milliString[4] = ".00";
char timeString[9] = "00:00:00";
char dateString[12] = "00 00 1970";
char previousTimeString[9] = "00:00:00";

time_t time;

DCF77 DCF = DCF77(DCF_PIN,DCF_INTERRUPT);

char timeStr[12] = "00:00:00.00"; // Variable for storing time in HH:MM:SS:SS format
char sd_string[4] = "..."; // Variable for storing SD card status
char cas_string[4] = "..."; // Variable for storing DCF77 signal status

char cas_od_spusteni[13]= "000:00:00.00"; //Predefined string for time since startup
char posledni_ulozeny_cas[12] ="           ";

void setup() {
  Serial.begin(9600); 
  DCF.Start(); // Start listening to DCF77 signal
}

void loop() {
  dcfCas();
  tvorbaStringu();
  setiny();
}

void casOdSpusteni() {
  // Function that stores the time since startup in the "cas_od_spusteni" variable
  // Called in the "void zmacknutitlacitka()" function
  unsigned long cas = millis();
  // Split time since startup into hours, minutes, seconds, and miliseconds
  unsigned int hodiny = cas / 3600000;
  unsigned int minuty = (cas / 60000) % 60;
  unsigned int sekundy = (cas / 1000) % 60;
  unsigned int setiny_sekundy = (cas / 10) % 100;
  // Format and store the time since startup in "cas_od_spusteni"
  sprintf(cas_od_spusteni, "%03d:%02d:%02d.%02d", hodiny, minuty, sekundy, setiny_sekundy);
}

void tvorbaStringu(){
  // funkce pro tvorbu stringÅ¯ s Äasem a datumem zÃ­skanÃ½mi z DCF77
  sprintf(timeString, "%02d:%02d:%02d", hour(), minute(), second());
  sprintf(dateString, "%02d %02d %04d", day(), month(), year());
}

void dcfCas(){
  // Function for updating the internal time of Arduino with DCF77
  time_t DCFtime = DCF.getTime(); // Returns the current time in CET
  //time_t DCFUTCtime = DCF.getTime(); // Returns the current time in UTC
  if (DCFtime != 0) { // if DCF is found set inner time to DCF time
    Serial.println(DCFtime); //when is time updateted print time in time_t format (Unix time)
    setTime(DCFtime); // 
    sprintf(cas_string, " OK");
    tvorbaStringu();
  }
}

void setiny() {
  // Function for creating miliseconds
  if (strcmp(timeString, previousTimeString) != 0) { // Compares time strings
    lastTime = millis();
    strcpy(previousTimeString, timeString); // Assigns the new time string to the previous time string
  }
  float elapsedTime = (float)(millis() - lastTime) / 1000; // Calculates the time since the strings differed
  snprintf(milliString, 5, "%02d", (int)(elapsedTime * 100) % 100); // Creates a string with milliseconds
  sprintf(timeStr, "%s.%s", timeString, milliString); // Joins the time string and the string with milliseconds
}

