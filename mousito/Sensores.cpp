#include "Sensores.h"
#include <SPI.h>
#include "VL6180X.h"
#include "Adafruit_VL53L0X.h"

boolean Sensores::addSensor(const uint8_t type, const char name[], const uint8_t address, const uint8_t pin){
  switch(type){
    case TYPE_VL53L0X:
      if (vl53l0x_count == MAX_SENSORES - 1) return false;
      vl53l0x[vl53l0x_count] = Adafruit_VL53L0X();
      strcpy(vl53l0x_names[vl53l0x_count], name);
      initializeVL53L0X(&vl53l0x[vl53l0x_count], address, pin);
      vl53l0x_count++;
      break;
    case TYPE_VL6180X:
      if (vl6180x_count == MAX_SENSORES - 1) return false;
      vl6180x[vl6180x_count] = VL6180X();
      strcpy(vl6180x_names[vl6180x_count], name);
      vl6180x_count++;
      break;
  }
  return true;
}

Adafruit_VL53L0X *Sensores::getVL53L0XSensor(const char name[]){
  uint8_t position = 0;
  while (position < vl53l0x_count){
    if (strcmp(vl53l0x_names[position], name) == 0){
      return &vl53l0x[position];
    }
    position++;
  }
  return NULL;
}

VL6180X *Sensores::getVL6180XSensor(const char name[]){
  uint8_t position = 0;
  while (position < vl6180x_count){
    if (strcmp(vl6180x_names[position], name) == 0){
      return &vl6180x[position];
    }
    position++;
  }
  return NULL;
}

void Sensores::initializeVL53L0X(Adafruit_VL53L0X *sensor, const uint8_t address, const uint8_t pin){
  
}

void Sensores::initializeVL6180X(VL6180X *sensor, const uint8_t address, const uint8_t pin){
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
  delay(DELAY_ARRANQUE_SENSOR);
  sensor->init();
  sensor->setAddress(address);
  sensor->configureDefault();
  sensor->setTimeout(500);
  delay(DELAY_ARRANQUE_SENSOR);
}

uint16_t Sensores::getDistance(const char name[]){
  Adafruit_VL53L0X *sensor53L;
  VL6180X *sensor618;
  switch(getType(name)){
    case TYPE_VL53L0X:
      sensor53L = getVL53L0XSensor(name);
      
      break;
    case TYPE_VL6180X:
      sensor618 = getVL6180XSensor(name);
      return sensor618->readRangeSingleMillimeters();
      break;
  }
  return 0;
}

uint8_t Sensores::getType(const char name[]){
  uint8_t position = 0;
  for (position = 0; position < vl53l0x_count; position++){
    if (strcmp(vl53l0x_names[position], name) == 0) return TYPE_VL53L0X;
  }
  for (position = 0; position < vl6180x_count; position++){
    if (strcmp(vl6180x_names[position], name) == 0) return TYPE_VL6180X;
  }
  return TYPE_NONE;
}