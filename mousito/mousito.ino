#include "Arduino.h"
#include "mousito.h"
#include "Sensores.h"
// #include "BluetoothSerial.h"

// BluetoothSerial BTSerial;
Sensores sensores;

void setup(){
  // BTSerial.begin("Mousito01BT");
  Serial.begin(115200);

  sensores.addSensor(TYPE_VL53L0X, "Primero");
  sensores.addSensor(TYPE_VL53L0X, "Segundo");
  sensores.addSensor(TYPE_VL6180X, "Tercero");
  sensores.addSensor(TYPE_VL6180X, "Cuarto");
  sensores.addSensor(TYPE_VL53L0X, "Quinto");
}

void loop(){
  sensores.getVL53L0XSensors(/* &BTSerial */);
  sensores.getVL6180XSensors(/* &BTSerial */);
  delay(1000);
}