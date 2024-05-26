#include "Display.h"
#include <U8glib.h>

extern char timeStr[12];
extern char sd_string[4];
extern char cas_string[4];
extern char posledni_ulozeny_cas[12];

U8GLIB_SSD1306_128X64 mujOled(U8G_I2C_OPT_NONE);

void Display_Init() {
  // Initialize display here if needed
}

void Display_Update() {
  zobrazitNaDisplay(timeStr, sd_string, cas_string, posledni_ulozeny_cas);
}

void zobrazitNaDisplay(const char* cas, const char* sd, const char* dcf, const char* zmereny_cas) {
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
