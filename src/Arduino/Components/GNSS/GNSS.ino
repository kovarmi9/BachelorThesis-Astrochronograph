# include "Arduino.h"

// definice struktury Cas
struct Cas {
  unsigned long den;
  unsigned long mesic;
  unsigned long rok;
  unsigned long hodina;
  unsigned long minuta;
  unsigned long sekunda;
};
// definice proměnné cas datového typu Cas
Cas cas;
// Definice proměnné cas_od_spusteni datového typu Cas
Cas cas_od_spusteni;
// nastavení počátečního času spučtění arduina
unsigned long startTime = 0;
// řetězec pro uložení času
char timeString[10];
// řetězec pro uložení předchozího času
char previousTimeString[10];
// řetězec pro uchování setin sekundy
char milliString[5];

void setup() {
  Serial.begin(9600);
  // čas kdy byl spuštěn program
  startTime = millis();
}

void loop() {
  // pokud je k dispozici seriový port
  while (Serial.available()) {
    // přečti řádek ze seriového portu
    String nmeaMessage = Serial.readStringUntil('\n');
    // pokud řádek začíná na GNZDA
    if (nmeaMessage.startsWith("$GNZDA")) {
      // konverze NMEA zprávy na na řetězec
      // funkce c_str() vrací ukazatel na pole znaků reprezentujících řetězec
      char* msg = nmeaMessage.c_str();
      // deklarace pole ukazatelů na znaky
      char* parts[15] = {};
      // definice první pozice
      int i = 0;
      // rozdělení řetězce na části oddělené oddělovačem ","
      parts[i] = strtok(msg, ",");
      // cyklus rozdělující řetězec a ukládající hodnoty z něj do parts
      while(parts[i] != NULL) {
        parts[++i] = strtok(NULL, ",");
      }
      // když řetězec není prázdný
      if (parts[1] != NULL) {
        String casString = String(parts[1]);
        // naplnění structu cas
        cas.hodina = casString.substring(0, 2).toInt();
        cas.minuta = casString.substring(2, 4).toInt();
        cas.sekunda = casString.substring(4, 6).toInt();
      }
      cas.den = atoi(parts[2]);
      cas.mesic = atoi(parts[3]);
      cas.rok = atoi(parts[4]);
    }
    sprintf(timeString, "%02lu:%02lu:%02lu", cas.hodina, cas.minuta, cas.sekunda);
    setiny(milliString);
    char dateString[12];
    sprintf(dateString, "%02lu %02lu %04lu", cas.den, cas.mesic, cas.rok);
    Serial.print(timeString);
    Serial.print(".");
    Serial.print(String(milliString));
    Serial.print(";");
    CasOdSpisteni();
  }
}

// argument funkce je ukazatel na milliString
void setiny(char* milliString) {
    // funkce pro tvorbu setin sekund
  static unsigned long lastTime = 0;
  if (strcmp(timeString, previousTimeString) != 0) {
    lastTime = millis();
    strcpy(previousTimeString, timeString);
  }
  float elapsedTime = (float)(millis() - lastTime) / 1000;
  snprintf(milliString, 5, "%02d", (int)(elapsedTime * 100) % 100);
}

void CasOdSpisteni() {
  // funkce která ukládá čas od spuštění do proměné "cas_od_spusteni"
  unsigned long elapsedTime = millis() - startTime;
  // Naplnění struktury cas_od_spusteni hodnotami
  cas_od_spusteni.hodina = elapsedTime / 3600000;
  cas_od_spusteni.minuta = (elapsedTime / 60000) % 60;
  cas_od_spusteni.sekunda = (elapsedTime / 1000) % 60;
  unsigned long setiny_sekundy = elapsedTime / 10;
  // tvorba řetězce času od spuštění
  char elapsedTimeString[15];
  sprintf(elapsedTimeString, "%02lu:%02lu:%02lu.%02lu", cas_od_spusteni.hodina, cas_od_spusteni.minuta, cas_od_spusteni.sekunda, setiny_sekundy);
  Serial.println(elapsedTimeString);
}
