const int channelPinA = 2;
const int channelPinB = 4;
const int motorPinRev = 5;
const int motorPinFwd = 6;

volatile long lastReadingTime = 0;
volatile long lastStoredTime = 0;
 
volatile int steps = 0;
volatile int lastSteps = 0;
float carSpeed = 0.0;
 
bool IsCW = true;

void setup()
{
   pinMode(channelPinA, INPUT_PULLUP);
   Serial.begin(9600);
   attachInterrupt(digitalPinToInterrupt(channelPinA), doEncodeA, CHANGE);
}
  
void loop()
{
  carSpeed = calculateSpeed();
  Serial.println(carSpeed);
  moveFwd(128);
  delay(1000);
  carSpeed = calculateSpeed();
  Serial.println(carSpeed);
  moveFwd(255);
  delay(1000);
  carSpeed = calculateSpeed();
  Serial.println(carSpeed);
  moveStop();
  delay(1000);
}

void moveFwd(int spd){
  analogWrite(motorPinRev, 0);
  analogWrite(motorPinFwd, spd);
}

void moveRev(int spd){
  analogWrite(motorPinRev, spd);
  analogWrite(motorPinFwd, 0);
}

void moveStop(){
  analogWrite(motorPinRev, 0);
  analogWrite(motorPinFwd, 0);
}

float calculateSpeed(){
  float result;
  int space;
  float timeLapsed;
  space = steps - lastSteps;
  lastReadingTime = millis();
  timeLapsed = lastReadingTime - lastStoredTime;
  result = space / timeLapsed;
  lastStoredTime = lastReadingTime;
  lastSteps = steps;
  return result;
}

void doEncodeA()
{
    if (digitalRead(channelPinA) == digitalRead(channelPinB))
    {
       steps++;
    }
    else
    {
       steps--;
    }
}
