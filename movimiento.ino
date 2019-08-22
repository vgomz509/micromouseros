const int motorLeftEncoderA = 3;
const int motorLeftEncoderB = 4;

const int motorRightEncoderA = 2;
const int motorRightEncoderB = 5;

const int motorLeftPinRev = 7;
const int motorLeftPinFwd = 6;

const int motorRightPinRev = 10;
const int motorRightPinFwd = 9;

const int axisDistance = 62; // En milímetros
const float wheelRatio = 360; //valor sacado empiricamente contando los pulsos por vuelta de rueda
const float wheelDiameter = 23.5; // En milímetros

const float KpVelLienal = 2;
const float KdVelLineal = 1;
const float KpVelAngular = 200;
const float KdVelAngular = 1;

const int MAX_SPEED = 215; //velocidad maxima de los motores en mm/s

volatile long lastReadingTimeLeft = 0;
volatile long lastStoredTimeLeft = 0;

volatile long lastReadingTimeRight = 0;
volatile long lastStoredTimeRight = 0;

volatile long stepsLeft = 0;
volatile long lastStepsLeft = 0;


volatile long stepsRight = 0;
volatile long lastStepsRight = 0;

volatile float desiredLinearSpeed =0;
volatile float desiredAngularSpeed = 0;

volatile int leftSPD = 0;
volatile int rightSPD = 0;


void setup()
{
   pinMode(motorLeftEncoderA, INPUT_PULLUP);
   pinMode(motorRightEncoderA, INPUT_PULLUP);
   Serial.begin(9600);
   attachInterrupt(digitalPinToInterrupt(motorLeftEncoderA), doEncodeLeft, RISING);
   attachInterrupt(digitalPinToInterrupt(motorRightEncoderA), doEncodeRight, RISING);
}
  
void loop()
{
  setSpeeds(MAX_SPEED, 0);
  delay(100);
//    Serial.print (right);
//    Serial.print (",");
//    Serial.print (left);
//    Serial.print (",");
//    Serial.print (carLinearSpeed(right, left));
//    Serial.print (",");
//    Serial.println (carAngularSpeed(right, left));
}

void setSpeeds(float linear, float angular){
  float right, left;
  float errorLinearSpeed = 0;
  float errorAngularSpeed = 0;
  right = rightSpeed();
  left = leftSpeed();
  errorLinearSpeed = linear - carLinearSpeed(right, left);
  errorAngularSpeed = angular - carAngularSpeed(right, left);
  leftSPD = (left + (errorLinearSpeed * KpVelLienal) - (errorAngularSpeed * KpVelAngular));
  if (leftSPD > MAX_SPEED){
    leftSPD = MAX_SPEED;
  }
  rightSPD = (right + (errorLinearSpeed * KpVelLienal) + (errorAngularSpeed * KpVelAngular)); 
  if (rightSPD > MAX_SPEED){
    rightSPD = MAX_SPEED;
  }
  
  moveLeft(255*(leftSPD/MAX_SPEED));
  moveRight(255*(rightSPD/MAX_SPEED));

  Serial.print (right);
  Serial.print (",");
  Serial.print (left);
  Serial.print (",");
  Serial.print (errorLinearSpeed);
  Serial.print (",");
  Serial.print (errorAngularSpeed);
  Serial.print (",");
  Serial.print (carLinearSpeed(right, left));
  Serial.print (",");
  Serial.println (carAngularSpeed(right, left));

//const float KdVelLineal = 1;
//const float  = 1;
//const float KdVelAngular = 1;

  
}

void moveLeft(int spd){
  if (spd == 0) {
    analogWrite(motorLeftPinRev, 0);
    analogWrite(motorLeftPinFwd, 0);  
  } else if (spd > 0) {
    analogWrite(motorLeftPinRev, 0);
    analogWrite(motorLeftPinFwd, spd);
  } else {
    analogWrite(motorLeftPinRev, spd*(-1));
    analogWrite(motorLeftPinFwd, 0);
  }
}

void moveRight(int spd){
  if (spd == 0){
    analogWrite(motorRightPinRev, 0);
    analogWrite(motorRightPinFwd, 0);
  } else if (spd > 0) {
    analogWrite(motorRightPinRev, 0);
    analogWrite(motorRightPinFwd, spd);
  } else {
    analogWrite(motorRightPinRev, spd*(-1));
    analogWrite(motorRightPinFwd, 0);
  }
}



float leftSpeed(){
  float result;
  float mm;
  int space;
  float timeLapsed;
  space = stepsLeft - lastStepsLeft;
  lastReadingTimeLeft = millis();
  timeLapsed = lastReadingTimeLeft - lastStoredTimeLeft;
  mm = (PI * wheelDiameter * space) / (wheelRatio);
  result = mm / (timeLapsed / 1000);
  lastStoredTimeLeft = lastReadingTimeLeft;
  lastStepsLeft = stepsLeft;
  return result;
}

float rightSpeed(){
  float result;
  float mm;
  int space;
  float timeLapsed;
  space = stepsRight - lastStepsRight;
  lastReadingTimeRight = millis();
  timeLapsed = lastReadingTimeRight - lastStoredTimeRight;
  mm = (PI * wheelDiameter * space) / (wheelRatio);
  result = mm / (timeLapsed / 1000);
  lastStoredTimeRight = lastReadingTimeRight;
  lastStepsRight = stepsRight;
  return result;
}

float carLinearSpeed(float right, float left){
  return ((right + left) / 2);
}

float carAngularSpeed(float right, float left){
  return ((right - left) / axisDistance);
}

void doEncodeLeft()
{
    if (digitalRead(motorLeftEncoderA) == digitalRead(motorLeftEncoderB))
    {
       stepsLeft++;
    }
    else
    {
       stepsLeft--;
    }
}

void doEncodeRight()
{
    if (digitalRead(motorRightEncoderA) == digitalRead(motorRightEncoderB))
    {
       stepsRight++;
    }
    else
    {
       stepsRight--;
    }
}
