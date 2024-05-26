/* Tento kÃ³d inicializuje vnitÅnÃ­ Äas arduina s Äasem zÃ­skanÃ½m z DCF77, pÅipojuje SD kartu a po zmÃ¡ÄknutÃ­ tlaÄÃ­tka na nÃ­ vytvÃ¡ÅÃ­ soubor Zaznam.txt do nÄjÅ¾ uklÃ¡dÃ¡ aktuÃ¡lnÃ­ Äas zÃ­skanÃ½ z DCF77 
 * ve formÃ¡tu HH:MM:SS.SS DD MM rok (00:00:00.00 01 01 1970) a Äas od spuÅ¡tÄnÃ­ arduina ve formÃ¡tu HHH:MM:SS.SS (000:00:00.00). Tento kod dÃ¡le zobrazuje na displej aktuÃ¡lnÃ­ Äas, poslednÃ­ zaznamenanÃ½ Äas, 
 * stav SD karty (SD:... / SD: OK) a stav inicializace Äasu z DCF77(DCF:... / DCF: OK)
 * 
 * TENTO KÃD BYL VYTVOÅEN PRO ARDUINO UNO!!!
 * 
 * kam co zapojit?
 * 
 * slot pro SD kartu:
 * MOSI - pin 11
 * MISO - pin 12
 * SCK  - pin 13
 * CS   - pin  4
 * tlaÄÃ­tko(rotaÄnÃ­ enkÃ³der):
 * SW   - pin  8
 * displej:
 * SCK  - pin A5
 * SDA  - pin A4
 * DCF77 pÅijÃ­maÄ:
 * TCO  - pin  2
 *
 *
 * popis DCF77 pÅijÃ­maÄe: https://www.pollin.de/p/dcf-77-empfangsmodul-dcf1-810054
 * popis displeje: https://dratek.cz/arduino/1479-iic-i2c-displej-oled-0.91-128x32-bily-3-3-v-5v-pro-iot-arduino-raspbery.html
 * popis SD karta: https://www.laskakit.cz/microsd-card-modul-spi/?gclid=EAIaIQobChMI4dXtueG2_QIVR-J3Ch1mqwbHEAQYASABEgI9BPD_BwE#relatedFiles
 * https://github.com/arduino-libraries/SD/tree/master/examples
 * popis tlaÄÃ­tko (rotaÄnÃ­ enkoder): https://dratek.cz/arduino/837-rotacni-enkoder.html
 *
 */
 
/* SOUÄASNÃ PROBLÃMY!!!
 * pÅi odpojenÃ­ SD karty nenÃ­ plynulÃ½ vÃ½pis na displej
 * ?pokud je Äas aktualizovÃ¡n v pÅÃ­padÄ Å¾e je odpojena SD karta tak se celÃ½ program zasekne
 * 90% na arduinu uno je celkem dost pamÄti... potÅeba optimalizace
 * funkce kterÃ© jsou volÃ¡ny v kodu jen jednou by bylo vhodnÃ© pÅedÄlat na inline funkce aby zabÃ­rali mÃ©nÄ mÃ­sta
 * kod by chtÄl jeÅ¡tÄ trochu zpÅehlednit
 */


// nedefinovÃ¡nÃ­ pinÅ¯
#define DCF_PIN 2 // Connection pin to DCF 77 device
#define DCF_INTERRUPT 0 // Interrupt number associated with pin
#define BUTTON_PIN 8 //definuje na kterÃ©m je sw u tlaÄÃ­tka
#define KARTA_CS_PIN 4 //definuje na kterÃ©m pinu je cs u SD krty

// knihovny
#include "Arduino.h" // C++ library for Wiring functionality
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include <DCF77.h> // Knihovna pro pÅipojenÃ­ k DCF 77
#include <TimeLib.h>// Knihovna pro prÃ¡ci s Äasem
#include <SD.h> // Knihovna pro prÃ¡ci s SD kartou
#include <U8glib.h> // Knihovna pro prÃ¡ci s OLED displejem

// PromÄnnÃ© pro DCF 77
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
DCF77 DCF = DCF77(DCF_PIN,DCF_INTERRUPT);

// PromÄnnÃ© pro OLED displej
U8GLIB_SSD1306_128X64 mujOled(U8G_I2C_OPT_NONE); // Objekt pro OLED displej
char timeStr[12] = "00:00:00.00"; //promÄnnÃ¡ pro uklÃ¡dÃ¡nÃ­ Äasu ve formÃ¡tu HH:MM:SS:SS
char sd_string[4] = "..."; //promÄnnÃ¡ pro uklÃ¡dÃ¡nÃ­ stavu SD karty
char cas_string[4] = "..."; //promÄnnÃ¡ pro uklÃ¡dÃ¡nÃ­ stavu signÃ¡lu DCF77

File myFile; //objekt pro prÃ¡ci se souborem na SD kartÄ

bool tlacitko_stisknuto = false; //stav tlaÄÃ­tka

char cas_od_spusteni[13]= "000:00:00.00"; //pÅeddefinovÃ¡nÃ­ stringu s Äasem od spuÅ¡tÄnÃ­
char posledni_ulozeny_cas[12] ="           ";

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
  // funkce pro kontrolu stavu SD karty
  if (!SD.begin(KARTA_CS_PIN)) {
    strcpy(sd_string, "...");
  } 
  else {
    strcpy(sd_string, " OK");
  }
}


void zobrazitNaDisplay(const char* cas, const char* sd, const char* dcf,const char* zmereny_cas) {
/* PouÅ¾itÃ© fonty:
 * fonty lze mÄnit podle: https://github.com/olikraus/u8glib/wiki/fontsize
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
  } while ( mujOled.nextPage() );
}

void zmacknutitlacitka(){
  // funkce pro rozpoznÃ¡nÃ­ zmÃ¡ÄknutÃ­ tlaÄÃ­tka a vykonÃ¡nÃ­ pÅÃ­kazÅ¯ po jeho zmÃ¡ÄknutÃ­
  if (digitalRead(BUTTON_PIN) == LOW) {
    if (!tlacitko_stisknuto) {
      tlacitko_stisknuto = true;
      PlaySound(2,30); // Call the PlaySound function with argument 
      casOdSpusteni(); // aktualizace cas_od_spusteni
      myFile = SD.open("ZAZNAM.txt", FILE_WRITE);
      if (myFile) {
        // pÅÃ­kazy vkonanÃ© po zmÃ¡ÄknutÃ­ tlaÄÃ­tka
        myFile.print(timeStr); // uklÃ¡dÃ¡d do ZAZANAM.txt Äas zÃ­skanÃ½ z DCF77 ve formÃ¡tu (HH:MM:SS.SS)
        myFile.print(" "); // pÅidÃ¡ mezeru
        myFile.print(dateString); // uklÃ¡dÃ¡ datum ve formÃ¡tu (DD MM rok)
        myFile.print("  "); // pÅidÃ¡ dvÄ mezery
        myFile.println(cas_od_spusteni);
        myFile.close();
        strcpy(posledni_ulozeny_cas, timeStr);//pÅiÅazenÃ­ hodnoty jednoho pole do druhÃ©ho pole
        // konec pÅÃ­kazÅ¯ vykonanÃ½ch po zmÃ¡ÄknutÃ­ tlaÄÃ­tka
        } else {
        while (1);
      }
    }
  } else {
    tlacitko_stisknuto = false; // zajiÅ¡Å¥uje aby jedno zmÃ¡ÄknutÃ­ tlaÄÃ­tka bylo brÃ¡no pouze jako jedno zmÃ¡ÄknutÃ­
  }
}

void casOdSpusteni() {
  // funkce kterÃ¡ uklÃ¡dÃ¡ Äas od spuÅ¡tÄnÃ­ do promÄnÃ© "cas_od_spusteni"
  // je volÃ¡na ve funkci "void zmacknutitlacitka()"
  unsigned long cas = millis();
  // rozdÄlenÃ­ Äasu od spuÄtÅ¡nÃ­ do hodin, minut, sekund a setin
  unsigned int hodiny = cas / 3600000;
  unsigned int minuty = (cas / 60000) % 60;
  unsigned int sekundy = (cas / 1000) % 60;
  unsigned int setiny_sekundy = (cas / 10) % 100;
  // naformÃ¡tÃ³vÃ¡nÃ­ a uloÅ¾enÃ­ Äasu od spuÅ¡tÄnÃ­ do "cas_od_spusteni"
  sprintf(cas_od_spusteni, "%03d:%02d:%02d.%02d", hodiny, minuty, sekundy, setiny_sekundy);
}


void tvorbaStringu(){
  // funkce pro tvorbu stringÅ¯ s Äasem a datumem zÃ­skanÃ½mi z DCF77
  sprintf(timeString, "%02d:%02d:%02d", hour(), minute(), second());
  sprintf(dateString, "%02d %02d %04d", day(), month(), year());
}

void dcfCas(){
  // funkce pro aktualizaci vnitÅnÃ­ho Äasu arduina s DCF77
  time_t DCFtime = DCF.getTime();
  if (DCFtime != 0) {
    setTime(DCFtime);
    sprintf(cas_string, " OK");
    tvorbaStringu();
    PlaySound(1,30); // Call the PlaySound function with argument 
    myFile = SD.open("TEST.txt", FILE_WRITE); // Open file to write to
    if (myFile) {
      myFile.print("cas naposledy aktualizovan: "); // provede zapis do souboru TEST.txt
      myFile.print(timeString); // uklÃ¡dÃ¡d do Cas_aktualizace.txt Äas zÃ­skanÃ½ z DCF77 ve formÃ¡tu (HH:MM:SS.SS)
      myFile.print(" ");
      myFile.println(dateString);
      myFile.close(); // Close file
    } else {
    while (1);
    }
  }
}

void setiny() {
  // funkce pro tvorbu setin sekund
  if (strcmp(timeString, previousTimeString) != 0) { //porovnÃ¡vÃ¡ ÄasovÃ© stringy
    lastTime = millis();
    strcpy(previousTimeString, timeString);//pÅiÅazuje do pÅedchozÃ­ho ÄasovÃ©ho stringu novÃ½ ÄasovÃ½ string
  }
  float elapsedTime = (float)(millis() - lastTime) / 1000; //vypoÄÃ­tÃ¡vÃ¡ Äas od doby, kdy se stringy liÅ¡ili
  snprintf(milliString, 5, "%02d", (int)(elapsedTime * 100) % 100); //vytvÃ¡ÅÃ­ string s milisekundami
  sprintf(timeStr, "%s.%s", timeString, milliString); //spojuje ÄasovÃ½ string a strig s milisekundami
}

// Function to play a sound
void PlaySound(int nazev, int volume){
  myDFPlayer.volume(volume); // Set the volume 0 - 30 
  myDFPlayer.play(nazev); // Play the mp3 file in the order corresponding to the order variable
  // zvuk byl vytvořen pomocí: https://ttsmaker.com/cs#google_vignette
}

void PlaySoundSetup(){
  mySoftwareSerial.begin(9600);
  if (!myDFPlayer.begin(mySoftwareSerial)) 
  {
    while (true);
  }
}