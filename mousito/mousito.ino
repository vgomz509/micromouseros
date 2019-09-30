#include "Arduino.h"
#include "mousito.h"
#include "Sensores.h"
// #include "BluetoothSerial.h"

// BluetoothSerial BTSerial;
Sensores sensores;

void setup(){
  // BTSerial.begin("Mousito01BT");
  Serial.begin(115200);

  sensores.addSensor(TYPE_VL53L0X, "Primero", 0, 0);
  sensores.addSensor(TYPE_VL53L0X, "Segundo", 0, 0);
  sensores.addSensor(TYPE_VL6180X, "Tercero", 0, 0);
  sensores.addSensor(TYPE_VL6180X, "Cuarto", 0, 0);
  sensores.addSensor(TYPE_VL53L0X, "Quinto", 0, 0);
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