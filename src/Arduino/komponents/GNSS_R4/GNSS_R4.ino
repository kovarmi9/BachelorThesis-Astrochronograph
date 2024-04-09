#include "Arduino.h"
//pro UNO R3 musí být Serial=Serial

// Definice struktury Cas
struct Cas {
  unsigned long den;
  unsigned long mesic;
  unsigned long rok;
  unsigned long hodina;
  unsigned long minuta;
  unsigned long sekunda;
};

// Definice globální proměnné pro čas a čas od spuštění
Cas cas;
Cas cas_od_spusteni;

// Nastavení počátečního času spuštění Arduina
unsigned long startTime = 0;

// Řetězec pro uložení času
char timeString[10];

// Řetězec pro uložení předchozího času
char previousTimeString[10];

// Řetězec pro uchování setin sekundy
char milliString[5];

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  // Nastavení času spuštění programu
  startTime = millis();
}

void loop() {
  // Pokud jsou k dispozici data na sériovém portu
  while (Serial1.available()) {
    // Přečtení NMEA zprávy do pole znaků
    char nmeaMessage[100];
    int messageIndex = 0;
    char incomingChar;

    while (Serial1.available() > 0) {
      incomingChar = Serial1.read();
      // Uložení přijatých znaků do pole
      nmeaMessage[messageIndex] = incomingChar;
      messageIndex++;
      // Přerušení smyčky při přijetí znaku nového řádku nebo překročení délky zprávy
      if (incomingChar == '\n' || messageIndex >= sizeof(nmeaMessage) - 1) {
        break;
      }
    }
    // Přidání ukončující nuly k ukončení řetězce
    nmeaMessage[messageIndex] = '\0';

    // Pokud zpráva začíná řetězcem "$GNZDA"
    if (strstr(nmeaMessage, "$GNZDA") != NULL) {
      char* parts[15] = {};
      int i = 0;
      // Rozdělení zprávy na části oddělené čárkami
      parts[i] = strtok(nmeaMessage, ",");
      while (parts[i] != NULL) {
        parts[++i] = strtok(NULL, ",");
      }
      // Uložení hodnoty času do struktury cas
      if (parts[1] != NULL) {
        cas.hodina = atoi(parts[1]);
        cas.minuta = atoi(parts[2]);
        cas.sekunda = atoi(parts[3]);
      }
      cas.den = atoi(parts[4]);
      cas.mesic = atoi(parts[5]);
      cas.rok = atoi(parts[6]);
    }

    // Formátování času do řetězce
    sprintf(timeString, "%02lu:%02lu:%02lu", cas.hodina, cas.minuta, cas.sekunda);
    // Volání funkce pro výpočet setin sekundy
    setiny(milliString);
    // Výpis času na sériový port
    Serial.print(timeString);
    Serial.print(".");
    Serial.print(String(milliString));
    Serial.print(";");
    // Volání funkce pro výpočet času od spuštění
    CasOdSpisteni();
  }
}

// Funkce pro výpočet setin sekundy
void setiny(char* milliString) {
  static unsigned long lastTime = 0;
  // Kontrola změny času
  if (strcmp(timeString, previousTimeString) != 0) {
    lastTime = millis();
    strcpy(previousTimeString, timeString);
  }
  // Výpočet uplynulého času v sekundách
  float elapsedTime = (float)(millis() - lastTime) / 1000;
  // Formátování setin sekundy
  snprintf(milliString, 5, "%02d", (int)(elapsedTime * 100) % 100);
}

// Funkce pro výpočet času od spuštění
void CasOdSpisteni() {
  // Výpočet uplynulého času v milisekundách
  unsigned long elapsedTime = millis() - startTime;
  // Naplnění struktury cas_od_spusteni hodnotami
  cas_od_spusteni.hodina = elapsedTime / 3600000;
  cas_od_spusteni.minuta = (elapsedTime / 60000) % 60;
  cas_od_spusteni.sekunda = (elapsedTime / 1000) % 60;
  // Výpočet setin sekundy
  unsigned long setiny_sekundy = elapsedTime / 10;
  // Formátování času od spuštění do řetězce
  char elapsedTimeString[15];
  sprintf(elapsedTimeString, "%02lu:%02lu:%02lu.%02lu", cas_od_spusteni.hodina, cas_od_spusteni.minuta, cas_od_spusteni.sekunda, setiny_sekundy);
  // Výpis času od spuštění na sériový port
  Serial.println(elapsedTimeString);
}
