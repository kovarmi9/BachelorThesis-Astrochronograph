// RGB LED modul KY-016

// nastavení vstupních pinů
#define pinR 9
#define pinG 10
#define pinB 6
// proměnná obsahující maximální svítivost LED diod
const int maxSvit = 255;

void setup() {
  // nastavení pinů pro LED jako výstup
  pinMode(pinR, OUTPUT);
  pinMode(pinG, OUTPUT);
  pinMode(pinB, OUTPUT);
  // otestování všech barev pomocí podprogramu test
  test(pinR);
  delay(500);
  test(pinG);
  delay(500);
  test(pinB);
  delay(500);
}

void loop() {
  // pomocí tří smyček v sobě dojde k postupnému nastavení různých
  // kombinací barev z RGB spektra, vždy je maximální svítivost
  // rozdělena na 5 stupňů - celkem tedy 5^3 = 125 kombinací
  for (int red = 0 ; red < maxSvit; red += maxSvit/5) {
    for (int green = 0 ; green < maxSvit; green += maxSvit/5) {
      for (int blue = 0 ; blue < maxSvit; blue += maxSvit/5) {
        // nastavení barvy na RGB LED modulu podle
        // aktuálních proměnnýc ve smyčkách
        nastavRGB(red, green, blue);
        // krátká pauza mezi barvami
        delay(200);
      }
    }
  }
  /*
    // nastavení červené barvy
    nastavRGB(maxSvit, 0  , 0  );
    // nastavení zelené barvy
    nastavRGB(0  , maxSvit, 0  );
    // nastavení modré barvy
    nastavRGB(0  , 0  , maxSvit);
    // nastavení bílé barvy
    nastavRGB(maxSvit, maxSvit, maxSvit);
  */
}
void test(int ledPin) {
  // nastavení intenzity na daný pin vzestupně od 0 do hodnoty maxSvit
  for (int intenzita = 0 ; intenzita <= maxSvit; intenzita += 5) {
    // řízení jasu pomocí PWM na daném pinu
    analogWrite(ledPin, intenzita);
    // krátké zpoždění pro lepší efekt
    delay(30);
  }
  // nastavení intenzity na daný pin sestupně od hodnoty maxSvit do 0
  for (int intenzita = maxSvit ; intenzita >= 0; intenzita -= 5) {
    // řízení jasu pomocí PWM na daném pinu
    analogWrite(ledPin, intenzita);
    // krátké zpoždění pro lepší efekt
    delay(30);
  }
}
void nastavRGB(int cervena, int zelena, int modra) {
  // nastavení všech barev na zvolené intenzity
  analogWrite(pinR, cervena);
  analogWrite(pinG, zelena);
  analogWrite(pinB, modra);
}