#include <Arduino.h>
#include "mousito.h"
#include "config.h"
#include "Sensores.h"
#include "Encoder.h"
// #include "BluetoothSerial.h"

// BluetoothSerial BTSerial;
Sensores sensores;
// Encoder encoderLeft = Encoder(LEFT_ENCODER_A_PIN, LEFT_ENCODER_B_PIN);
// Encoder encoderRight = Encoder(RIGHT_ENCODER_A_PIN, RIGHT_ENCODER_B_PIN);
Encoder *encoderLeft, *encoderRight;

void setup(){
  // BTSerial.begin("Mousito01BT");
  Serial.begin(115200);

  sensores.addSensor(TYPE_VL53L0X, "Primero", 0, 0);
  sensores.addSensor(TYPE_VL53L0X, "Segundo", 0, 0);
  sensores.addSensor(TYPE_VL6180X, "Tercero", 0, 0);
  sensores.addSensor(TYPE_VL6180X, "Cuarto", 0, 0);
  sensores.addSensor(TYPE_VL6180X, "Quinto", 0, 0);

  // attachInterrupt(LEFT_ENCODER_A_PIN, doEncoderLeft, CHANGE);
  // attachInterrupt(RIGHT_ENCODER_A_PIN, doEncoderRight, CHANGE);
  encoderLeft = new Encoder(LEFT_ENCODER_A_PIN, LEFT_ENCODER_B_PIN, &doEncoderRight);
  encoderRight = new Encoder(RIGHT_ENCODER_A_PIN, RIGHT_ENCODER_B_PIN, &doEncoderRight);
}

void loop(){
  Serial.println("INICIO PRUEBAS");
  if (sensores.getVL53L0XSensor("Primero") != NULL){
    Serial.println("Primera prueba OK");
  }
  if (sensores.getVL53L0XSensor("Tercero") == NULL){
    Serial.println("Segunda prueba OK");
  }
  if (sensores.getVL6180XSensor("Tercero") != NULL){
    Serial.println("Tercera prueba OK");
  }
  Serial.println("FIN PRUEBAS");
  delay(1000);
}

void doEncoderLeft(){
  encoderLeft->signalReceived();
}

void doEncoderRight(){
  encoderRight->signalReceived();
}