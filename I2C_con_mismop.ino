/************************************************************************
 Based on :
  M5StackFire I2C Scanner

  The M5StackFire has a connector for I2C devices. 
  This program scans the addresses 1-127 continuosly and shows 
  the devices found on the TFT.

  The M5Stack fire has two internal I2C devices at address 0x68 and 0x75.
  
  If they do not appear on the TFT it could mean you made a short cut on 
  the I2C bus.

  October 2018, ChrisMicro
*******************
 Code to enable n diferent VL6180X sensor on same I2C Bus changing its addresses
 
************************************************************************/

#include <M5StickC.h>
#include <VL6180X.h>
#include <Wire.h>


VL6180X dist1;
VL6180X dist2;

void setup() 
{
  Wire.begin();
  Serial.begin(115200);
  M5.begin();


  pinMode(G26, OUTPUT);
  
  M5.Lcd.setRotation(0);
  M5.Lcd.fillScreen( BLACK );
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.setTextColor(YELLOW);  
  M5.Lcd.setTextSize(4);

  M5.Lcd.fillScreen( BLACK );
  M5.Lcd.setCursor(0, 0);
  digitalWrite(G26,LOW);  //desactivar sensor 2
  dist1.init();// init 1 en la addr por defecto
  dist1.setAddress(0x39); //le cambia la dirección de sensor 1 a 39hex
  dist1.configureDefault();
  dist1.setTimeout(500);
  
  digitalWrite(G26,HIGH); //activar sensor 2
  delay(200);//dale un tiempo antes de iniciarlo... si no no funciona
  dist2.init();
  dist2.setAddress(0x49);//le cambia la dirección a 49hex
  dist2.configureDefault();
  dist2.setTimeout(500);
  

  M5.Lcd.fillScreen( BLACK );
}

int textColor=YELLOW;

void loop() 
{
  M5.Lcd.fillScreen( BLACK );
  M5.Lcd.setCursor(0, 0);
  
  M5.Lcd.println(dist1.readRangeSingleMillimeters());
  M5.Lcd.println(dist2.readRangeSingleMillimeters());
  
  Serial.println(dist1.readRangeSingleMillimeters());
  Serial.println(dist2.readRangeSingleMillimeters());
  
  delay(500);

}


void scan(){
    byte error, address;
  int nDevices;
 
  Serial.println("Scanning...");
 
  nDevices = 0;
  for(address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
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
