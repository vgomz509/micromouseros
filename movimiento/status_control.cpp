//#include <util/delay.h>
//#include <math.h>
#include "Arduino.h"
#include "config.h"

//DEFINICIÓN DE VARIABLES
volatile double myPosx = 0.0;
volatile double myPosy = 0.0;
volatile double myAlfa = 0.0;
volatile double myLastPosx = 0.0;
volatile double myLastPosy = 0.0;
volatile double myLastAlfa = 0.0;
volatile float myLinearSpeed = 0.0;
volatile float myAngularSpeed =0.0;
volatile float myLinearAceleration = 0.0;
volatile float myAngularAceleration =0.0;

volatile uint8_t myCasillaX = 0;
volatile uint8_t myCasillaY = 0;
volatile uint8_t myOrientacion = 0;

volatile long pulsesLeft = 0;
volatile long lastPulsesLeft = 0;

volatile long pulsesRight = 0;
volatile long lastPulsesRight = 0;

volatile uint8_t myMode = 0;
volatile unsigned long lastStoredTime = 0;

//funciones internas


void reseteaPosicion_xya(double x, double y, double a){ 

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
  double myIncrementAlfa;

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
 
void setMode(uint8_t newMode){
  myMode = newMode;
}

void setwhereami_cx (uint8_t newcx){
  myCasillaX = newcx;
}

void setwhereami_cy (uint8_t newcy){
  myCasillaY = newcy;
}

void setwhereami_co (uint8_t newco){
  myOrientacion = newco;
}


//GETTERS

uint8_t getwhereami_cx (){ //devuelve un byte con la casilla X en la que se encuentra el robot
  return myCasillaX;
} 
uint8_t getwhereami_cy (){ //devuelve un byte con la casilla Y en la que se encuentra el robot
  return myCasillaY;
} 
uint8_t getwhereami_co (){ //devuelve un byte con la orientacion en la que se encuentra el robot (0=norte 1=sur 2=este 3=oeste)
  return myOrientacion;
} 

double getwhereami_x (){ // devuelve un float indicando la posicion dentro de la casilla
  return myPosx;
}

double getwhereami_y (){ // devuelve un float indicando la posicion dentro de la casilla
  return myPosy;
}

double getwhereami_a (){ // devuelve un float indicando la orientaci�n dentro de la casilla 
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

uint8_t getMode(){
  return myMode;
}
