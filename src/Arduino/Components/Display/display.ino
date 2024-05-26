#include "Display.h"

unsigned long currentMillis = 0;

char timeStr[] = "00:00:00.00";
char sd_string[] = " OK";
char cas_string[] = "...";
char zmereny_cas[] = "00:00:00.00";

void setup() {
  display_setup();
}

void loop() {
  display(timeStr, sd_string, cas_string, zmereny_cas);
}

