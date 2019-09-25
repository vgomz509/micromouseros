#include "Arduino.h"
#include "config.h"
#include "motion_control.h"
#include "status_control.h"
#include "display.h"
#include "U8g2lib.h"
#include "BluetoothSerial.h"
#include "ota_server.h"

Display *display;
// TaskHandle_t otaServer;
BluetoothSerial BTSerial;

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

  // createOTAServer(&otaServer);
  BTSerial.begin("Mousito01BT");
 }

void enviaDatosSerie() {
  actualiza();
  BTSerial.print(getwhereami_x());
  BTSerial.print (";");
  BTSerial.print(getwhereami_y());
  BTSerial.print (";");
  BTSerial.println(getwhereami_a());
}

void loop()
{
  // delay(1000);
  // setMode(display->selectMode());

  setMode(JUANJO);
  // avanza(2);
  // fullStop ();
  // display->showIP(getIP());
  // delay(2000);

  enviaDatosSerie();
  display->showPosition();
  delay(2000);
}
