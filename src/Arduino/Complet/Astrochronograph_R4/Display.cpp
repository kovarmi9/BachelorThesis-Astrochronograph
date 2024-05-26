#include <U8g2lib.h>
#include "Display.h"

// Inicializace displeje s pomocí softwarového I2C
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);

char timeStr[12] = "00:00:00.00"; //promÄnnÃ¡ pro uklÃ¡dÃ¡nÃ­ Äasu ve formÃ¡tu HH:MM:SS:SS
char sd_string[4] = "..."; //promÄnnÃ¡ pro uklÃ¡dÃ¡nÃ­ stavu SD karty
char cas_string[4] = "..."; //promÄnnÃ¡ pro uklÃ¡dÃ¡nÃ­ stavu signÃ¡lu DCF77

void display_setup() {
  u8g2.begin();
}

void zobrazitNaDisplay(const char* cas, const char* sd, const char* dcf,const char* zmereny_cas) {
  // fonty lze mÄnit podle: https://github.com/olikraus/u8g2/wiki
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

