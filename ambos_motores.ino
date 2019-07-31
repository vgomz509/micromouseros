const int motorLeftEncoderA = 2;
const int motorLeftEncoderB = 4;

const int motorRightEncoderA = 3;
const int motorRightEncoderB = 7;

const int motorLeftPinRev = 5;
const int motorLeftPinFwd = 6;

const int motorRightPinRev = 9;
const int motorRightPinFwd = 10;

volatile long lastReadingTimeLeft = 0;
volatile long lastStoredTimeLeft = 0;

volatile long lastReadingTimeRight = 0;
volatile long lastStoredTimeRight = 0;

volatile int stepsLeft = 0;
volatile int laststepsLeft = 0;
float carSpeedLeft = 0.0;

volatile int stepsRight = 0;
volatile int lastStepsRight = 0;
float carSpeedRight = 0.0;

void setup()
{
   pinMode(motorLeftEncoderA, INPUT_PULLUP);
   pinMode(motorRightEncoderA, INPUT_PULLUP);
   Serial.begin(9600);
   attachInterrupt(digitalPinToInterrupt(motorLeftEncoderA), doEncodeLeft, CHANGE);
   attachInterrupt(digitalPinToInterrupt(motorRightEncoderA), doEncodeRight, CHANGE);
}
  
void loop()
{
  carSpeedLeft = calculateLeftSpeed();
  carSpeedRight = calculateRightSpeed() + 20.0;
  Serial.print(carSpeedLeft);
  Serial.print(",");
  Serial.println(carSpeedRight);
  moveLeftFwd(128);
  moveRightFwd(128);
  delay(1000);
  carSpeedLeft = calculateLeftSpeed();
  carSpeedRight = calculateRightSpeed() + 20.0;
  Serial.print(carSpeedLeft);
  Serial.print(",");
  Serial.println(carSpeedRight);
  moveLeftFwd(255);
  moveRightFwd(255);
  delay(1000);
  carSpeedLeft = calculateLeftSpeed();
  carSpeedRight = calculateRightSpeed() + 20.0;
  Serial.print(carSpeedLeft);
  Serial.print(",");
  Serial.println(carSpeedRight);
  moveLeftStop();
  moveRightStop();
  delay(1000);
}

void moveLeftFwd(int spd){
  analogWrite(motorLeftPinRev, 0);
  analogWrite(motorLeftPinFwd, spd);
}

void moveLeftRev(int spd){
  analogWrite(motorLeftPinRev, spd);
  analogWrite(motorLeftPinFwd, 0);
}

void moveLeftStop(){
  analogWrite(motorLeftPinRev, 0);
  analogWrite(motorLeftPinFwd, 0);
}

void moveRightFwd(int spd){
  analogWrite(motorRightPinRev, 0);
  analogWrite(motorRightPinFwd, spd);
}

void moveRightRev(int spd){
  analogWrite(motorRightPinRev, spd);
  analogWrite(motorRightPinFwd, 0);
}

void moveRightStop(){
  analogWrite(motorRightPinRev, 0);
  analogWrite(motorRightPinFwd, 0);
}

float calculateLeftSpeed(){
  float result;
  int space;
  float timeLapsed;
  space = stepsLeft - laststepsLeft;
  lastReadingTimeLeft = millis();
  timeLapsed = lastReadingTimeLeft - lastStoredTimeLeft;
  result = space / timeLapsed;
  lastStoredTimeLeft = lastReadingTimeLeft;
  laststepsLeft = stepsLeft;
  return result;
}

float calculateRightSpeed(){
  float result;
  int space;
  float timeLapsed;
  space = stepsRight - lastStepsRight;
  lastReadingTimeRight = millis();
  timeLapsed = lastReadingTimeRight - lastStoredTimeRight;
  result = space / timeLapsed;
  lastStoredTimeRight = lastReadingTimeRight;
  lastStepsRight = stepsRight;
  return result;
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
