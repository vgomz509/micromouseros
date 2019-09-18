//#include <util/delay.h>
//#include <math.h>
#include "Arduino.h"
#include "config.h"

//DEFINICIÓN DE VARIABLES
volatile float myPosx = 0.0;
volatile float myPosy = 0.0;
volatile float myAlfa = 0.0;
volatile float myLastPosx = 0.0;
volatile float myLastPosy = 0.0;
volatile float myLastAlfa = 0.0;
volatile float myLinearSpeed = 0.0;
volatile float myAngularSpeed =0.0;
volatile float myLinearAceleration = 0.0;
volatile float myAngularAceleration =0.0;

volatile byte myCasillaX = 0;
volatile byte myCasillaY = 0;
volatile byte myOrientacion = 0;

volatile long pulsesLeft = 0;
volatile long lastPulsesLeft = 0;

volatile long pulsesRight = 0;
volatile long lastPulsesRight = 0;

volatile byte myMode = 0;
volatile unsigned long lastStoredTime = 0;

//funciones internas


void reseteaPosicion_xya(float x, float y, float a){ 

  myPosx = x;
  myPosy = y;
  myAlfa = a;
  myLastPosx = x;
  myLastPosy = y;
  myLastAlfa = a;
}

void actualiza(){  
  float leftSpeed;
  float rightSpeed;
  float timeLapsed;
  float myAvance;
  float myIncrementAlfa;

  myLastAlfa= myAlfa;
  myLastPosx = myPosx;
  myLastPosy = myPosy;
  myAvance = ((pulsesLeft-lastPulsesLeft)+ (pulsesRight-lastPulsesRight)/2) * PULSES_TO_MM;
  myAlfa = myLastAlfa + ((((pulsesRight-lastPulsesRight) - (pulsesLeft-lastPulsesLeft)) / AXIS_DISTANCE))* PULSES_TO_MM;
  myIncrementAlfa = (myAlfa - myLastAlfa);
  myPosx = myLastPosx - myAvance * sin(myAlfa); 
  myPosy = myLastPosy + myAvance * cos(myAlfa); 
  
  timeLapsed = millis() - lastStoredTime;
  lastStoredTime = millis();
  myLinearAceleration = ((myAvance * 1000 / timeLapsed) - myLinearSpeed)/timeLapsed;
  myAngularAceleration = ((1000 * myIncrementAlfa / timeLapsed) - myAngularSpeed)/timeLapsed;
  myLinearSpeed = myAvance * 1000 / timeLapsed;
  myAngularSpeed = 1000 * myIncrementAlfa / timeLapsed; //rad/seg

  lastPulsesLeft = pulsesLeft;
  lastPulsesRight = pulsesRight;
}


void doEncodeLeft()
{
    if (digitalRead(LEFT_ENCODER_A_PIN) == digitalRead(LEFT_ENCODER_B_PIN))
    {
       pulsesLeft++;
    }
    else
    {
       pulsesLeft--;
    }
}

void doEncodeRight()
{
    if (digitalRead(RIGHT_ENCODER_A_PIN) == digitalRead(RIGHT_ENCODER_B_PIN))
    {
       pulsesRight++;
    }
    else
    {
       pulsesRight--;
    }
}

//SETTERS
 
void setMode(byte newMode){
  myMode = newMode;
}

void setwhereami_cx (byte newcx){
  myCasillaX = newcx;
}

void setwhereami_cy (byte newcy){
  myCasillaY = newcy;
}

void setwhereami_co (byte newco){
  myOrientacion = newco;
}


//GETTERS

byte getwhereami_cx (){ //devuelve un byte con la casilla X en la que se encuentra el robot
  return myCasillaX;
} 
byte getwhereami_cy (){ //devuelve un byte con la casilla Y en la que se encuentra el robot
  return myCasillaY;
} 
byte getwhereami_co (){ //devuelve un byte con la orientacion en la que se encuentra el robot (0=norte 1=sur 2=este 3=oeste)
  return myOrientacion;
} 

float getwhereami_x (){ // devuelve un float indicando la posicion dentro de la casilla
  return myPosx;
}

float getwhereami_y (){ // devuelve un float indicando la posicion dentro de la casilla
  return myPosy;
}

float getwhereami_a (){ // devuelve un float indicando la orientaci�n dentro de la casilla 
  return myAlfa;
}


boolean hayParedIzquierda (){
  
}
boolean hayParedDelante (){
  
}
boolean hayParedDerecha (){
  
}

float getLinearSpeed(){
  return myLinearSpeed;   
}
float getAngularSpeed(){
  return myAngularSpeed;
}

float getLinearAceleration(){
  return myLinearAceleration;
}

byte getMode(){
  return myMode;
}
