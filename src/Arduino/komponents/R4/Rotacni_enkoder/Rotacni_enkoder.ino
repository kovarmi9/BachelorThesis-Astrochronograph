// Rotační enkodér KY-040

// proměnné pro nastavení propojovacích pinů
int pinCLK = 6;
int pinDT  = 5;
int pinSW  = 4;

// proměnné pro uložení pozice a stavů pro určení směru
// a stavu tlačítka
int poziceEnkod = 0;
int stavPred;
int stavCLK;
int stavSW;

// proměnná pro sledování, zda bylo tlačítko uvolněno
bool tlacitko_uvolneno = true;

void setup() {
  // komunikace přes sériovou linku rychlostí 9600 baud
  Serial.begin(9600);
  TlacitkoSetup();
} 

void loop() {
  TlacitkoLoop();
}

void TlacitkoSetup() {
  // nastavení propojovacích pinů jako vstupních
  pinMode(pinCLK, INPUT);
  pinMode(pinDT, INPUT);
  // nastavení propojovacího pinu pro tlačítko
  // jako vstupní s pull up odporem
  pinMode(pinSW, INPUT_PULLUP);
  // načtení aktuálního stavu pinu CLK pro porovnávání
  stavPred = digitalRead(pinCLK);   
}

void TlacitkoLoop() {
  // načtení stavu pinu CLK
  stavCLK = digitalRead(pinCLK);
  // pokud je stav CLK odlišný od předchozího měření,
  // víme, že osa byla otočena
  if (stavCLK != stavPred) {
    // pokud stav pinu DT neodpovídá stavu pinu CLK,
    // byl pin CLK změněn jako první a rotace byla
    // po směru hodin, tedy vpravo
    if (digitalRead(pinDT) != stavCLK) {
      // vytištění zprávy o směru rotace a přičtení
      // hodnoty 1 u počítadla pozice enkodéru
      Serial.print("Rotace vpravo => | ");
      poziceEnkod ++;
    }
    // v opačném případě, tedy pin DT byl změněn
    // jako první, se jedná o rotaci
    // proti směru hodin, tedy vlevo
    else {
      // vytištění zprávy o směru rotace a odečtení
      // hodnoty 1 u počítadla pozice enkodéru
      Serial.print("Rotace vlevo  <= | ");
      poziceEnkod--;
    }
    // vytištění aktuální hodnoty pozice enkodéru
    Serial.print("Pozice enkoderu: ");
    Serial.println(poziceEnkod);
  }
  // uložení posledního stavu pinu CLK
  // jako reference pro další porovnávání
  stavPred = stavCLK;
  // načtení stavu pinu SW - tlačítko
  stavSW = digitalRead(pinSW);
  // v případě stisknutí vytiskni informaci
  // po sériové lince
  if (stavSW == 0 && tlacitko_uvolneno) {
    Serial.println("Stisknuto tlacitko enkoderu!");
    tlacitko_uvolneno = false;
    delay(500);
  } else if (stavSW == 1) {
    tlacitko_uvolneno = true;
  }
}
