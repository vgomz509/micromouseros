#include "Motores.h"
#include "Sensores.h"
#include "Encoder.h"
#include "config.h"

Motores::Motores(byte mode, Sensores *sensors, Encoder *encoderLeft, Encoder *encoderRight){
  myMode = mode;
  mySensors = sensors;
  myEncoderLeft = encoderLeft;
  myEncoderRight = encoderRight;
  myX = 0;
  myY = 0;
  myAlpha = 0;
  lastX = 0;
  lastY = 0;
  lastAlpha = 0;
}

void Motores::moveLeftMotor (int speed){
  if (speed == 0) {
    ledcWrite(LEFT_MOTOR_REV_CH, 0);
    ledcWrite(LEFT_MOTOR_FWD_CH, 0);  
  } else if (speed > 0) {
    ledcWrite(LEFT_MOTOR_REV_CH, 0);
    ledcWrite(LEFT_MOTOR_FWD_CH, speed);
  } else {
    ledcWrite(LEFT_MOTOR_REV_CH, speed*(-1));
    ledcWrite(LEFT_MOTOR_FWD_CH, 0);
  }
}

void moveRightMotor (int speed){
  if (speed == 0){
    ledcWrite(RIGHT_MOTOR_REV_CH, 0);
    ledcWrite(RIGHT_MOTOR_FWD_CH, 0);
  } else if (speed > 0) {
    ledcWrite(RIGHT_MOTOR_REV_CH, 0);
    ledcWrite(RIGHT_MOTOR_FWD_CH, speed);
  } else {
    ledcWrite(RIGHT_MOTOR_REV_CH, speed*(-1));
    ledcWrite(RIGHT_MOTOR_FWD_CH, 0);
  }
}

void Motores::avanza (byte casillas){
  switch (myMode) {
    case RECONOCIMIENTO:
      break;
    case CARRERA:
      break;
    case VUELTA_A_CASA:
      break;
    case TESTEO:
      moveLeftMotor(210); //70
      moveRightMotor(255); //55
      delay(775 * casillas);
      fullStop (); 
      delay (500);
      break;
    case VICTOR:
      float errorSpeed = 0.0;
      float alphaObjetivo = 0.0;
      float speedObjetivo = REC_SPEED;
      float errorAlpha = 0.0;
      float errorAlphaAcomulado = 0.0;
      float PIDSpeed;
      float PIDAngularPosition;
      while (myY < DIMENSION_CELDA * casillas){
        
        //pensamientos para control del angulo en rectas
        //en rectas apuntaremos a 2/3 del final de la recta(s), una vez ahí nuestro alfaobjetivo será 0
        //pensamientos para control del angulo en curvas
        //se puede ir incrementan errorAlpha en las cuvas para tener una aceleración angular controlada
        //esta aceleración puede ser inversamente proporcional a la velocidad que llevamos en ese momento, por lo que podemos no tener una aceleración lineal
        // hasta que whereami_a+ errorAlpha sean igual o superior a alphaObjetivo, en ese momento ir a buscar el alpha objetivo, que este será angulo entre el vector del coche y el vector que forma coche con punto de salida de la curva
        
        //calculo errorAlpha para rectas
        //Ir a buscar el punto getwhereami_x =0 getwhereami_y =(DIMENSION_CELDA * N_casillas) - 45.0) una vez llegado a  DIMENSION_CELDA * N_casillas - 60.0 iremos a buscar el getwhereami_x =0 getwhereami_y =(DIMENSION_CELDA * N_casillas) 
        
        if (myY < (DIMENSION_CELDA * casillas - 60)){
          alphaObjetivo = (float)atan2 (myX, (DIMENSION_CELDA * casillas - myY - 45.0)); // arc tangent of /__________ cateto opuesto/ cateto contiguo
        }else{
          alphaObjetivo = (float)atan2 (myX, (DIMENSION_CELDA * casillas - myY)); // arc tangent of /__________ cateto opuesto/ cateto contiguo
        }
        
        errorSpeed = (REC_SPEED - myLinearSpeed);
        errorAlpha = (alphaObjetivo - myAlpha);
        errorAlphaAcomulado = errorAlphaAcomulado + errorAlpha;
        PIDSpeed = (errorSpeed * KP_SPEED_LINEAR) + myLinearAccel * KD_SPEED_LINEAR ;
        PIDAngularPosition = (errorAlpha * KP_SPEED_ANGULAR) + (errorAlphaAcomulado * KI_SPEED_ANGULAR) + (myLinearAccel* KD_SPEED_ANGULAR);  

        moveLeftMotor(map((constrain((REC_SPEED  - PIDAngularPosition), 0, TOP_SPEED)),0 ,TOP_SPEED, 0,255));
        moveRightMotor(map((constrain((REC_SPEED  + PIDAngularPosition), 0, TOP_SPEED)),0 ,TOP_SPEED, 0,255));
        Serial.print(myAlpha);
        Serial.print(errorAlpha);
        Serial.print(alphaObjetivo);
        Serial.println(PIDAngularPosition);

        actualiza();
      }
      break;
  }

/* 
///////////////////////////////////////////////////////////////////////////////////////////
  ESTA PARTE DEBE IMPLEMENTARSE EN LA CLASE MICROMOUSE
///////////////////////////////////////////////////////////////////////////////////////////
  switch (myOrientation ()) {//actualiza la posición y orientación en el laberinto
    case NORTE:    
      setwhereami_cy (getwhereami_cy () + N_casillas);
      break;
    case SUR:
      setwhereami_cy (getwhereami_cy () - N_casillas);
      break;
    case ESTE:
      setwhereami_cx (getwhereami_cx () + N_casillas);
      break;
    case OESTE:
      setwhereami_cx (getwhereami_cx () - N_casillas);
      break;
  } 
///////////////////////////////////////////////////////////////////////////////////////////
  HASTA AQUÍ
///////////////////////////////////////////////////////////////////////////////////////////
*/

  myY = myY - (casillas * DIMENSION_CELDA);
  // reseteaPosicion_xya(getwhereami_x(), getwhereami_y()-N_casillas * DIMENSION_CELDA, getwhereami_a());
}

void Motores::actualiza (){
  float leftSpeed;
  float rightSpeed;
  float timeLapsed;
  float myAvance;
  double myAlphaIncrement;

  // ******* CALCULAR CUANTO TARDA EN HACER LA FUNCIÓN
  long start = micros();

  lastAlpha = myAlpha;
  lastX = myX;
  lastY = myY;
  myAvance = ((myEncoderLeft->getPulses()-myEncoderLeft->getLastPulses())+ (myEncoderRight->getPulses()-myEncoderRight->getLastPulses())/2) * PULSES_TO_MM;
  myAlpha = lastAlpha + ((((myEncoderRight->getPulses()-myEncoderRight->getLastPulses()) - (myEncoderLeft->getPulses()-myEncoderLeft->getLastPulses())) / AXIS_DISTANCE))* PULSES_TO_MM;
  myAlphaIncrement = (myAlpha - lastAlpha);
  myX = lastX - myAvance * sin(myAlpha); 
  myY = lastY + myAvance * cos(myAlpha); 
  
  timeLapsed = millis() - lastStoredTime;
  lastStoredTime = millis();
  myLinearAccel = ((myAvance * 1000 / timeLapsed) - myLinearSpeed) / timeLapsed;
  myAngularAccel = ((1000 * myAlphaIncrement / timeLapsed) - myAngularSpeed) / timeLapsed;
  myLinearSpeed = myAvance * 1000 / timeLapsed;
  myAngularSpeed = 1000 * myAlphaIncrement / timeLapsed; //rad/seg

  myEncoderLeft->savePulses();
  myEncoderRight->savePulses();
  
  // ******* CALCULAR CUANTO TARDA EN HACER LA FUNCIÓN
  long finish = micros();
  Serial.println(finish - start);
}
