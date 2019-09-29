#ifndef sensores_h
  #define sensores_h

  #include "VL6180X.h"
  #include "Adafruit_VL53L0X.h"
  #include "BluetoothSerial.h"

  // Definiciones para los tipos de sensores
  #define TYPE_VL6180X 1
  #define TYPE_VL53L0X 2

  class Sensores{
    public:
      boolean addSensor(uint8_t type, const char *name);
      void getVL53L0XSensors(/* BluetoothSerial *BTSerial */);
      void getVL6180XSensors(/* BluetoothSerial *BTSerial */);
    private:
      Adafruit_VL53L0X vl53l0x[10];
      VL6180X vl6180x[10];
      uint8_t vl53l0x_count = 0;
      uint8_t vl6180x_count = 0;
      char vl53l0x_names[10][20];
      char vl6180x_names[10][20];
  };
#endif