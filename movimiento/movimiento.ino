#include "arduino.h"
#include "config.h"
#include "motion_control.h"
#include "status_control.h"

void setup()
{
   pinMode(LEFT_ENCODER_A_PIN, INPUT_PULLUP);
   pinMode(RIGHT_ENCODER_A_PIN, INPUT_PULLUP);
   Serial.begin(BAUD_RATE);
   attachInterrupt(LEFT_ENCODER_A_PIN, doEncodeLeft, CHANGE);
   attachInterrupt(RIGHT_ENCODER_A_PIN, doEncodeRight, CHANGE);
}

void enviaDatosSerie(){
  
  Serial.print(getwhereami_cx());
  Serial.print (";");
  Serial.print(getwhereami_cy());
  Serial.print (";");
  Serial.println(getwhereami_co());
//  Serial.print ("encoder R:");
//  Serial.print (R);
//  Serial.print ("     encoder L:");
//  Serial.println (L);
}

void loop()
{
  setMode(VICTOR_TEST);
  avanza(1);
  fullStop ();
  delay (5000);

}
