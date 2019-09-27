#include "Arduino.h"
#include "config.h"
#include "motion_control.h"
#include "status_control.h"
#include <VL6180X.h>
#include "Adafruit_VL53L0X.h"
#include <Wire.h>


//#include "display.h"
//#include "U8g2lib.h"
#include "BluetoothSerial.h"
//#include "ota_server.h"

//Display *display;
// TaskHandle_t otaServer;

BluetoothSerial BTSerial;
VL6180X lateralDerecha;
VL6180X lateralIzquierda;
VL6180X diagonalDerecha;
VL6180X diagonalIzquierda;
Adafruit_VL53L0X frontalDerecha = Adafruit_VL53L0X(); 
Adafruit_VL53L0X frontalIzquierda = Adafruit_VL53L0X(); 
int mm1=255;
int mm2=255;
int mm3=255;
int mm4=255;

void setup()
{
  Serial.begin(BAUD_RATE);
  BTSerial.begin("Mousito01BT");
  Wire.begin();

  pinMode(ENABLE_SENSOR_LATERAL_IZQUIERDA,OUTPUT);//s2 6180 verdes
  pinMode(ENABLE_SENSOR_DIAGONAL_IZQUIERDA,OUTPUT);//s3 6180
  pinMode(ENABLE_SENSOR_DIAGONAL_DERECHA,OUTPUT);//s4 6180
  pinMode(ENABLE_SENSOR_FRONTAL_DERECHA,OUTPUT);//s5 53L0 lilas
  pinMode(ENABLE_SENSOR_FRONTAL_IZQUIERDA,OUTPUT);//s6 53L0

  digitalWrite(ENABLE_SENSOR_LATERAL_IZQUIERDA,LOW);
  digitalWrite(ENABLE_SENSOR_DIAGONAL_IZQUIERDA,LOW);
  digitalWrite(ENABLE_SENSOR_DIAGONAL_DERECHA,LOW);
  digitalWrite(ENABLE_SENSOR_FRONTAL_DERECHA,LOW);
  digitalWrite(ENABLE_SENSOR_FRONTAL_IZQUIERDA,LOW);

  lateralDerecha.init();
  lateralDerecha.setAddress(0x51);
  lateralDerecha.configureDefault();
  lateralDerecha.setTimeout(500);
  delay(DELAY_ARRANQUE_SENSOR);

  digitalWrite(ENABLE_SENSOR_LATERAL_IZQUIERDA,HIGH);
  delay(DELAY_ARRANQUE_SENSOR);
  lateralIzquierda.init();
  lateralIzquierda.setAddress(0x52);
  lateralIzquierda.configureDefault();
  lateralIzquierda.setTimeout(500);
  delay(DELAY_ARRANQUE_SENSOR);

  digitalWrite(ENABLE_SENSOR_DIAGONAL_IZQUIERDA,HIGH);
  delay(DELAY_ARRANQUE_SENSOR);
  diagonalIzquierda.init();
  diagonalIzquierda.setAddress(0x53);
  diagonalIzquierda.configureDefault();
  diagonalIzquierda.setTimeout(500);
  delay(DELAY_ARRANQUE_SENSOR);

  digitalWrite(ENABLE_SENSOR_DIAGONAL_DERECHA,HIGH);
  delay(DELAY_ARRANQUE_SENSOR);
  diagonalDerecha.init();
  diagonalDerecha.setAddress(0x54);
  diagonalDerecha.configureDefault();
  diagonalDerecha.setTimeout(500);
  delay(DELAY_ARRANQUE_SENSOR);

//  digitalWrite(ENABLE_SENSOR_FRONTAL_DERECHA,HIGH);
//  delay(DELAY_ARRANQUE_SENSOR);
//  //Serial.println(s5.begin(0x55));
//  frontalDerecha.setAddress(0x55);
//  delay(DELAY_ARRANQUE_SENSOR);
//
//  digitalWrite(ENABLE_SENSOR_FRONTAL_IZQUIERDA,HIGH);
//  delay(DELAY_ARRANQUE_SENSOR);
//  //Serial.println(s5.begin(0x56));
//  frontalIzquierda.setAddress(0x56);
//  delay(DELAY_ARRANQUE_SENSOR);

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
  
  
  attachInterrupt(LEFT_ENCODER_A_PIN, doEncodeLeft, CHANGE);
  attachInterrupt(RIGHT_ENCODER_A_PIN, doEncodeRight, CHANGE);

//  display = new Display(/* rotation=*/U8G2_R0, /* clock=*/ OLED_SCL, /* data=*/ OLED_SDA, /* reset=*/ OLED_RST);

  // createOTAServer(&otaServer);
  
 }

void enviaDatosSerie() {
  actualiza();
  BTSerial.print("sensor;"); 
  BTSerial.print(mm1>255?255:mm1);
  BTSerial.print(",");
  BTSerial.print(mm2>255?255:mm2);
  BTSerial.print(",");
  BTSerial.print(mm3>255?255:mm3);
  BTSerial.print(",");
  BTSerial.print(mm4>255?255:mm4);
  //BTSerial.print(getwhereami_x());
  //BTSerial.print (";");
  //BTSerial.print(getwhereami_y());
  BTSerial.print ("left;");
  BTSerial.print (getPulsesLeft());
  BTSerial.print ("right;");
  BTSerial.println (getPulsesRight());
  //BTSerial.print (";");
  //BTSerial.println(getwhereami_a());
}

void loop()
{
  
  mm1=lateralDerecha.readRangeSingleMillimeters();
  mm2=lateralIzquierda.readRangeSingleMillimeters();
  mm3=diagonalIzquierda.readRangeSingleMillimeters();
  mm4=diagonalDerecha.readRangeSingleMillimeters();
//  int mm5=255;
//  int mm6=255;
//
//  VL53L0X_RangingMeasurementData_t measure;
//  VL53L0X_RangingMeasurementData_t measure2;
//  frontalDerecha.rangingTest(&measure, false);
//   if (measure.RangeStatus != 4) {
//      mm5=measure.RangeMilliMeter;
//        }
//  frontalIzquierda.rangingTest(&measure2, false);
//  if (measure2.RangeStatus != 4) {
//      mm6=measure2.RangeMilliMeter;
//      }

//  Serial.print(",");
//  Serial.print(mm5>255?255:mm5);
//  Serial.print(",");
//  Serial.println(mm6>255?255:mm6);


  // delay(1000);
  // setMode(display->selectMode());
  //setMode(VICTOR);
  //avanza(2);
  //fullStop ();
  // display->showIP(getIP());
  // delay(2000);
  enviaDatosSerie();
  //  display->showPosition();
  //delay(2000);
}
