#include "display.h"
#include "config.h"
/* #include "U8g2lib.h"  //pantalla
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
 */
// U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ OLED_SCL, /* data=*/ OLED_SDA, /* reset=*/ OLED_RST); // For TTGo ESP32 LoRa-OLED  RMB

Display::Display(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE){
  _u8g2 = new U8G2_SSD1306_128X64_NONAME_F_SW_I2C(rotation, clock, data, reset); // For TTGo ESP32 LoRa-OLED  RMB
  _u8g2->begin();
}

/* void Display::showPosition()
{
  _u8g2->clearBuffer();
  _u8g2->setDisplayRotation(U8G2_R0);   // Se puede poner en horizontal cambiando a U8G2_R1 pero hay que tener cuidado con las posiciones al dibujar
  _u8g2->setFont(u8g2_font_6x10_tf);
  _u8g2->setFontRefHeightExtendedText();
  _u8g2->setDrawColor(1);
  _u8g2->setFontPosTop();
  _u8g2->setFontDirection(1);
  _u8g2->drawStr(128, 0, "CASILLA:");
  _u8g2->drawStr(110, 0, "X=");
  _u8g2->drawStr(110, 25, String(getwhereami_cx()).c_str()); // Método alternativo a las dos instrucciones comentadas a continuación.
//   _u8g2->setCursor(110, 25);
//   _u8g2->print(getwhereami_cx());
  _u8g2->drawStr(110, 32, "|Y=");
  _u8g2->setCursor(110, 55);
  _u8g2->print(getwhereami_cy());
  _u8g2->drawStr(100, 0, "ORIENTACIO:");
  switch (getwhereami_cy()) {
    case NORTE:
      _u8g2->drawStr(90, 0, "NORTE");
      break;
    case SUR:
      _u8g2->drawStr(90, 0, "SUR");
      break;
    case ESTE:
      _u8g2->drawStr(90, 0, "ESTE");
      break;
    case OESTE:
      _u8g2->drawStr(90, 0, "OESTE");
      break;
  }
  _u8g2->drawStr(80, 0, "MODO:");
  switch (getMode()) {
    case RECONOCIMIENTO:
      _u8g2->drawStr(70, 0, "RECONOCER");
      break;
    case CARRERA:
      _u8g2->drawStr(70, 0, "CARRERA");
      break;
    case VUELTA_A_CASA:
      _u8g2->drawStr(70, 0, "VUELTA_C");
      break;
    case TESTEO:
      _u8g2->drawStr(70, 0, "TESTEO");
      break;
    case JUANJO:
      _u8g2->drawStr(70, 0, "JUANJO");
      break;
    case SERGIO:
      _u8g2->drawStr(70, 0, "SERGIO");
      break;
    case VICTOR:
      _u8g2->drawStr(70, 0, "VICTOR");
      break;
  }
  _u8g2->sendBuffer();
} */

uint8_t Display::selectFromMenu(String menu[], uint8_t elements)
{
  int position = 0;
  long initTime, endTime;
  printMenu(menu, elements, position);
  for(;;){
    initTime = millis();
    while (digitalRead(0) == LOW){
      if (millis() - initTime > 1000) break;
    }
    endTime = millis();
    if (endTime - initTime < 1000 && endTime - initTime > 100){
      position++;
      if (position > elements - 1) position = 0;
      printMenu(menu, elements, position);
      delay(200);
    } else if (endTime - initTime > 1000){
      return position;
    }
  }
}

void Display::printMenu(String items[], uint8_t elements, uint8_t selected)
{
  _u8g2->clearBuffer();
  _u8g2->setDisplayRotation(U8G2_R0);
  _u8g2->setFont(u8g2_font_6x10_tf);
  _u8g2->setFontRefHeightExtendedText();
  _u8g2->setDrawColor(1);
  _u8g2->setFontPosTop();
  _u8g2->setFontDirection(1);
  
  uint8_t x = 0;
  uint8_t y = 128;

  for (uint8_t index = 0; index < elements; index++){
    if (index == selected){
      _u8g2->drawStr(y, x, ">");
      _u8g2->drawStr(y, x + 10, items[index].c_str());
    } else {
      _u8g2->drawStr(y, x, items[index].c_str());
    }
    y -= 10;
  }
  
  _u8g2->sendBuffer();
}

void Display::showIP(String address){
  _u8g2->clearBuffer();
  _u8g2->setDisplayRotation(U8G2_R1);
  _u8g2->setFont(u8g2_font_6x10_tf);
  _u8g2->setFontRefHeightExtendedText();
  _u8g2->setDrawColor(1);
  _u8g2->setFontPosTop();
  _u8g2->setFontDirection(1);
  _u8g2->drawStr(64, 0, "IP:");
  _u8g2->drawStr(54, 0, address.c_str());
  _u8g2->sendBuffer();
}