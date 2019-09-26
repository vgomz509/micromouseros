#include <Wire.h>
#include <VL6180X.h>
#include "Adafruit_VL53L0X.h"

VL6180X s1; //libre
VL6180X s2; //RX
VL6180X s3; //TX
VL6180X s4; //00
Adafruit_VL53L0X s5 = Adafruit_VL53L0X(); //23
Adafruit_VL53L0X s6 = Adafruit_VL53L0X(); //17

void setup() 
{  
  
Wire.begin();
Serial.begin(115200);
int dy=500;

Serial.println("Empezamos...");
for (int n=0; n<  
6;n++){
  Serial.println(n);
  delay(dy);
  }
scan();
Serial.println("Apago los CE...");
//pinMode(25,OUTPUT);//s2 6180
//pinMode(13,OUTPUT);//s3 6180
pinMode(00,INPUT);//s4 6180
//pinMode(23,OUTPUT);//s5 53L0
//pinMode(17,OUTPUT);//s6 53L0

//digitalWrite(25,LOW);
//digitalWrite(13,LOW);
//digitalWrite(00,LOW);
//digitalWrite(23,LOW);
//digitalWrite(17,LOW);
scan();

Serial.println("Arranco s1(6180)...");
//Arranco s1 
s1.init();
s1.setAddress(0x51);
s1.configureDefault();
s1.setTimeout(500);
delay(dy);
scan();

Serial.println("Arranca s2(6180)...");
//Arranco s2 
//digitalWrite(25,HIGH);

while (digitalRead(00)==true){}
delay(dy);
s2.init();
s2.setAddress(0x52);
s2.configureDefault();
s2.setTimeout(500);
delay(dy);
scan();

Serial.println("Arranca s3(6180)...");
//Arranco s3 
//digitalWrite(13,HIGH);
while (digitalRead(00)==true){}
delay(dy);
s3.init();
s3.setAddress(0x53);
s3.configureDefault();
s3.setTimeout(500);
delay(dy);
scan();

Serial.println("Arranca s4(6180)...");
//Arranco s4 
//digitalWrite(00,HIGH);
while (digitalRead(00)==true){}
delay(dy);
s4.init();
s4.setAddress(0x54);
s4.configureDefault();
s4.setTimeout(500);
delay(dy);
scan();

Serial.println("Arranca s5(53)...");
//Arranco s5 
//digitalWrite(23,HIGH);
while (digitalRead(00)==true){}
delay(dy);
Serial.println(s5.begin(0x55));
//s5.setAddress(0x55);
delay(dy);
scan();

Serial.println("Arranca s6(53)...");
//Arranco s6 
//digitalWrite(17,HIGH);
while (digitalRead(00)==true){}
delay(dy);
Serial.println(s6.begin(0x56));


delay(dy);
scan();

}


void loop() 
{
  int mm1=s1.readRangeSingleMillimeters();
  int mm2=s2.readRangeSingleMillimeters();
  int mm3=s3.readRangeSingleMillimeters();
  int mm4=s4.readRangeSingleMillimeters();
  int mm5=255;
  int mm6=255;

  VL53L0X_RangingMeasurementData_t measure;
  VL53L0X_RangingMeasurementData_t measure2;
  s5.rangingTest(&measure, false);
   if (measure.RangeStatus != 4) {
      mm5=measure.RangeMilliMeter;
        }
  s6.rangingTest(&measure2, false);
  if (measure2.RangeStatus != 4) {
      mm6=measure2.RangeMilliMeter;
      } 
  Serial.print(mm1>255?255:mm1);
  Serial.print(",");
  Serial.print(mm2>255?255:mm2);
  Serial.print(",");
  Serial.print(mm3>255?255:mm3);
  Serial.print(",");
  Serial.print(mm4>255?255:mm4);
  Serial.print(",");
  Serial.print(mm5>255?255:mm5);
  Serial.print(",");
  Serial.println(mm6>255?255:mm6);
}


void scan(){
   byte error, address;
  int nDevices;
 
  Serial.println("Scanning...");
 
  nDevices = 0;
  for(address = 1; address < 127; address++ )
  {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
 
    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");
 
      nDevices++;
    }
    else if (error==4)
    {
      Serial.print("Unknown error at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
  }
