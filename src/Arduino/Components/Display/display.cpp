#include <U8g2lib.h>

#include "Display.h"

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE); // Software I2C

void display_setup() {
  u8g2.begin();
}

void display(char timeStr[], char sd_string[], char cas_string[], char zmereny_cas[]) {
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_9x18_tr);
    u8g2.drawStr(5, 15, timeStr);
    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(5, 60, zmereny_cas);
    u8g2.drawStr(92, 30, "SD:");
    u8g2.drawStr(110, 30, sd_string);
    u8g2.drawStr(86, 40, "DCF:");
    u8g2.drawStr(110, 40, cas_string);
    u8g2.drawStr(86, 50, "GNSS:");
    u8g2.drawStr(86, 60, "NTP:");
  } while (u8g2.nextPage());
}
