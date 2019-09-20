#include "arduino.h"
#include "config.h"
#include "motion_control.h"
#include "status_control.h"
#include "U8g2lib.h"  //pantalla
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ OLED_SCL, /* data=*/ OLED_SDA, /* reset=*/ OLED_RST); // For TTGo ESP32 LoRa-OLED  RMB

void setup()
{
  pinMode(LEFT_ENCODER_A_PIN, INPUT_PULLUP);
  pinMode(RIGHT_ENCODER_A_PIN, INPUT_PULLUP);
  pinMode(LEFT_ENCODER_B_PIN, INPUT_PULLUP);
  pinMode(RIGHT_ENCODER_B_PIN, INPUT_PULLUP);

  ledcSetup(LEFT_MOTOR_REV_CH, FRECUENCIA_PWM, RESOLUCION_N_BITS_PWM);
  ledcSetup(LEFT_MOTOR_FWD_CH, FRECUENCIA_PWM, RESOLUCION_N_BITS_PWM);
  ledcSetup(RIGHT_MOTOR_REV_CH, FRECUENCIA_PWM, RESOLUCION_N_BITS_PWM);
  ledcSetup(RIGHT_MOTOR_FWD_CH, FRECUENCIA_PWM, RESOLUCION_N_BITS_PWM);
  
  ledcAttachPin(LEFT_MOTOR_REV_PIN, LEFT_MOTOR_REV_CH);
  ledcAttachPin(LEFT_MOTOR_FWD_PIN, LEFT_MOTOR_FWD_CH);
  ledcAttachPin(RIGHT_MOTOR_REV_PIN, RIGHT_MOTOR_REV_CH);
  ledcAttachPin(RIGHT_MOTOR_FWD_PIN, RIGHT_MOTOR_FWD_CH);
  
  Serial.begin(BAUD_RATE);
  attachInterrupt(LEFT_ENCODER_A_PIN, doEncodeLeft, CHANGE);
  attachInterrupt(RIGHT_ENCODER_A_PIN, doEncodeRight, CHANGE);
  u8g2.begin();
}

void muestraEnPantalla() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.setFontRefHeightExtendedText();
  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();
  u8g2.setFontDirection(1);
  u8g2.drawStr(128, 0, "CASILLA:");
  u8g2.drawStr(110, 0, "X=");
  u8g2.setCursor(110, 25);
  u8g2.print(getwhereami_cx());
  u8g2.drawStr(110, 32, "|Y=");
  u8g2.setCursor(110, 55);
  u8g2.print(getwhereami_cy());
  u8g2.drawStr(100, 0, "ORIENTACIO:");
  switch (getwhereami_cy()) {
    case NORTE:
      u8g2.drawStr(90, 0, "NORTE");
      break;
    case SUR:
      u8g2.drawStr(90, 0, "SUR");
      break;
    case ESTE:
      u8g2.drawStr(90, 0, "ESTE");
      break;
    case OESTE:
      u8g2.drawStr(90, 0, "OESTE");
      break;
  }
  u8g2.drawStr(80, 0, "MODO:");
  switch (getMode()) {
    case RECONOCIMIENTO:
      u8g2.drawStr(70, 0, "RECONOCER");
      break;
    case CARRERA:
      u8g2.drawStr(70, 0, "CARRERA");
      break;
    case VUELTA_A_CASA:
      u8g2.drawStr(70, 0, "VUELTA_C");
      break;
    case TESTEO:
      u8g2.drawStr(70, 0, "TESTEO");
      break;
    case JUANJO:
      u8g2.drawStr(70, 0, "JUANJO");
      break;
    case SERGIO:
      u8g2.drawStr(70, 0, "SERGIO");
      break;
    case VICTOR:
      u8g2.drawStr(70, 0, "VICTOR");
      break;
  }
  u8g2.sendBuffer();
}

void enviaDatosSerie() {

  Serial.print(getwhereami_cx());
  Serial.print (";");
  Serial.print(getwhereami_cy());
  Serial.print (";");
  Serial.println(getwhereami_co());
  //  Serial.print ("encoder R:");
  //  Serial.print (R);
  //  Serial.print ("     encoder L:");
  //  Serial.println (L);
}

void loop()
{
  setMode(TESTEO);
  //  avanza(1);
  
  muestraEnPantalla();
  
  fullStop ();
  delay (5000);

}
