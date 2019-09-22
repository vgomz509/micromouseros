#ifndef display_h
  #define display_h
  #include "U8g2lib.h"  //pantalla
  #ifdef U8X8_HAVE_HW_SPI
    #include <SPI.h>
  #endif
    #ifdef U8X8_HAVE_HW_I2C
    #include <Wire.h>
  #endif

  class Display
  {
    public:
      Display(const u8g2_cb_t *, uint8_t, uint8_t, uint8_t);
      void showPosition();
      uint8_t selectMode();
    private:
      U8G2_SSD1306_128X64_NONAME_F_SW_I2C *_u8g2;
      void printMenu(String [], uint8_t, uint8_t);
  };
#endif
