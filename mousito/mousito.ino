#include "Arduino.h"
#include "mousito.h"
#include "Sensores.h"
#include <VL6180X.h>
#include "Adafruit_VL53L0X.h"
#include <Wire.h>
// #include "BluetoothSerial.h"

// BluetoothSerial BTSerial;
Sensores sensores;

void setup(){
  // BTSerial.begin("Mousito01BT");
  Serial.begin(115200);
  sensores.addSensor(TYPE_VL53L0X, "123456");
  sensores.addSensor(TYPE_VL53L0X, "234567");
  sensores.addSensor(TYPE_VL6180X, "345678");
  sensores.addSensor(TYPE_VL6180X, "456789");
  sensores.addSensor(TYPE_VL53L0X, "567890");
}

void loop(){
  sensores.getVL53L0XSensors(/* &BTSerial */);
  sensores.getVL6180XSensors(/* &BTSerial */);
  delay(1000);
}