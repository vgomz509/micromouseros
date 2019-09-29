#include "Sensores.h"
#include "VL6180X.h"
#include "Adafruit_VL53L0X.h"
#include "BluetoothSerial.h"

boolean Sensores::addSensor(uint8_t type, const char name[]){
  switch(type){
    case TYPE_VL53L0X:
      if (vl53l0x_count == 9) return false;
      vl53l0x[vl53l0x_count] = Adafruit_VL53L0X();
      strcpy(vl53l0x_names[vl53l0x_count], name);
      vl53l0x_count++;
      break;
    case TYPE_VL6180X:
      if (vl6180x_count == 9) return false;
      vl6180x[vl6180x_count] = VL6180X();
      strcpy(vl6180x_names[vl6180x_count], name);
      vl6180x_count++;
      break;
  }
  return true;
}

void Sensores::getVL53L0XSensors(/* BluetoothSerial *BTSerial */){
  Serial.println("vl53l0x");
  for (int x = 0; x < vl53l0x_count; x++){
    Serial.println(vl53l0x_names[x]);
  }
}

void Sensores::getVL6180XSensors(/* BluetoothSerial *BTSerial */){
  Serial.println("vl6180x");
  for (int x = 0; x < vl6180x_count; x++){
    Serial.println(vl6180x_names[x]);
  }
}