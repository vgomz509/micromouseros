#include "Arduino.h"
#include "config.h"
#include "motion_control.h"
#include "status_control.h"
#include "display.h"
#include "U8g2lib.h"
#include "ota_server.h"

Display *display;
TaskHandle_t otaServer;

void setup()
{
  pinMode(0, INPUT);
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

  display = new Display(/* rotation=*/U8G2_R0, /* clock=*/ OLED_SCL, /* data=*/ OLED_SDA, /* reset=*/ OLED_RST);

  createOTAServer(&otaServer);
 }

void enviaDatosSerie() {
  actualiza();
  Serial.print(getwhereami_x());
  Serial.print (";");
  Serial.print(getwhereami_y());
  Serial.print (";");
  Serial.println(getwhereami_a());
  Serial.println(xPortGetCoreID());
}

void loop()
{
  //delay(1000);
  // enviaDatosSerie();
  // setMode(display->selectMode());

  setMode(JUANJO);
  // avanza(2);
  // fullStop ();
  display->showIP(getIP());
  delay(2000);

  display->showPosition();
  delay(2000);
}
