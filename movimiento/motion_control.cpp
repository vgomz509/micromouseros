//#include <util/delay.h>
//#include <math.h>
#include "Arduino.h"
#include "config.h"
#include "status_control.h"

//definición de variables



//FUNCIONES INTERNAS
void moveLeft(int spd){
  if (spd == 0) {
    ledcWrite(LEFT_MOTOR_REV_CH, 0);
    ledcWrite(LEFT_MOTOR_FWD_CH, 0);  
  } else if (spd > 0) {
    ledcWrite(LEFT_MOTOR_REV_CH, 0);
    ledcWrite(LEFT_MOTOR_FWD_CH, spd);
  } else {
    ledcWrite(LEFT_MOTOR_REV_CH, spd*(-1));
    ledcWrite(LEFT_MOTOR_FWD_CH, 0);
  }
}

void moveRight(int spd){
  if (spd == 0){
    ledcWrite(RIGHT_MOTOR_REV_CH, 0);
    ledcWrite(RIGHT_MOTOR_FWD_CH, 0);
  } else if (spd > 0) {
    ledcWrite(RIGHT_MOTOR_REV_CH, 0);
    ledcWrite(RIGHT_MOTOR_FWD_CH, spd);
  } else {
    ledcWrite(RIGHT_MOTOR_REV_CH, spd*(-1));
    ledcWrite(RIGHT_MOTOR_FWD_CH, 0);
  }
}


//FUNCIONES EXTERNAS

void fullStop (){
  moveLeft(0); 
  moveRight(0);
}

void avanza (byte N_casillas){ // avanza N casillas

  switch (getMode()) {
    case RECONOCIMIENTO:
      break;
    case CARRERA:
      break;
    case VUELTA_A_CASA:
      break;
    case TESTEO:
      moveLeft(70); //70
      moveRight(55); //55
      delay(775 * N_casillas);
      fullStop (); 
      delay (500);
      break;
//    case VICTOR_TEST:
//      float errorSpeed = 0.0;
//      float alfaObjetivo = 0.0;
//      float speedObjetivo = REC_SPEED;
//      float errorAlfa = 0.0;
//      float errorAlfaAcomulado = 0.0;
//      float PIDSpeed;
//      float PIDAngularPosition;
//      while (getwhereami_y ()< DIMENSION_CELDA * N_casillas){
//        
//        //pensamientos para control del angulo en rectas
//        //en rectas apuntaremos a 2/3 del final de la recta(s), una vez ahí nuestro alfaobjetivo será 0
//        //pensamientos para control del angulo en curvas
//        //se puede ir incrementan errorAlfa en las cuvas para tener una aceleración angular controlada
//        //esta aceleración puede ser inversamente proporcional a la velocidad que llevamos en ese momento, por lo que podemos no tener una aceleración lineal
//        // hasta que whereami_a+ errorAlfa sean igual o superior a alfaObjetivo, en ese momento ir a buscar el alfa objetivo, que este será angulo entre el vector del coche y el vector que forma coche con punto de salida de la curva
//        
//        //calculo errorAlfa para rectas
//        //Ir a buscar el punto getwhereami_x =0 getwhereami_y =(DIMENSION_CELDA * N_casillas) - 45.0) una vez llegado a  DIMENSION_CELDA * N_casillas - 60.0 iremos a buscar el getwhereami_x =0 getwhereami_y =(DIMENSION_CELDA * N_casillas) 
//        
//        if (getwhereami_y ()< (DIMENSION_CELDA * N_casillas - 60)){
//          alfaObjetivo = (float)atan2 ((double)getwhereami_x(),  (double)(DIMENSION_CELDA * N_casillas - getwhereami_y() - 45.0)); // arc tangent of /__________ cateto opuesto/ cateto contiguo
//        }else{
//          alfaObjetivo = (float)atan2 (getwhereami_x(),  (DIMENSION_CELDA * N_casillas - getwhereami_y())); // arc tangent of /__________ cateto opuesto/ cateto contiguo
//        }
//        
//        
//        
//        errorSpeed =(REC_SPEED - getLinearSpeed());
//        errorAlfa = (alfaObjetivo - getwhereami_a());
//        errorAlfaAcomulado = errorAlfaAcomulado + errorAlfa;
//        PIDSpeed = (errorSpeed * KP_SPEED_LINEAR) + getLinearAceleration() * KD_SPEED_LINEAR ;
//        PIDAngularPosition = (errorAlfa * KP_SPEED_ANGULAR) + (errorAlfaAcomulado * KI_SPEED_ANGULAR) + (getLinearAceleration()* KD_SPEED_ANGULAR);  
//        
//
//       
//        
//        
//
//        moveLeft(map((constrain((REC_SPEED  - PIDAngularPosition), 0, TOP_SPEED)),0 ,TOP_SPEED, 0,255));
//        moveRight(map((constrain((REC_SPEED  + PIDAngularPosition), 0, TOP_SPEED)),0 ,TOP_SPEED, 0,255));
//        Serial.print(getwhereami_a());
//        Serial.print(errorAlfa);
//        Serial.print(alfaObjetivo);
//        Serial.println(PIDAngularPosition);
//
//        actualiza();
//      }
//      break;
  }

  switch (getwhereami_co ()) {//actualiza la posición y orientación en el laberinto
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
  reseteaPosicion_xya(getwhereami_x(), getwhereami_y()-N_casillas * DIMENSION_CELDA, getwhereami_a());
}

void giraDerecha (){
  reseteaPosicion_xya(0.0,0.0,0.0);
  switch (getMode()) {
    case RECONOCIMIENTO:
      break;
    case CARRERA:
      break;
    case VUELTA_A_CASA:
      break;
    case TESTEO:
      moveLeft(70); //70
      moveRight(35); //55
      delay(760);
      fullStop (); 
      delay (500);       
      break;
//    case VICTOR_TEST:
//      float errorSpeed = 0.0;
//      float alfaObjetivo = -1.5708; //corresponde a girar pi/2 hacia la derecha los angulos en arduino están en radianes
//      float speedObjetivo = REC_SPEED;
//      float errorAlfa = 0.0;
//      float errorAlfaAcomulado = 0.0;
//      float PIDSpeed;
//      float PIDAngularPosition;
//      while (getwhereami_x ()< 90.0){
//
//        //se puede ir incrementan errorAlfa en las cuvas para tener una aceleración angular controlada
//        //esta aceleración puede ser inversamente proporcional a la velocidad que llevamos en ese momento, por lo que podemos no tener una aceleración lineal
//        // hasta que whereami_a+ errorAlfa sean igual o superior a alfaObjetivo, en ese momento ir a buscar el alfa objetivo, que este será angulo entre el vector del coche y el vector que forma coche con punto de salida de la curva
//        
//        //calculo errorAlfa para curvas a derecha
//        
//        errorSpeed =(REC_SPEED - getLinearSpeed());
//        errorAlfa = (alfaObjetivo - getwhereami_a());
//        errorAlfaAcomulado = errorAlfaAcomulado + errorAlfa;
//        PIDSpeed = (errorSpeed * KP_SPEED_LINEAR) + getLinearAceleration() * KD_SPEED_LINEAR ;
//        PIDAngularPosition = (errorAlfa * KP_SPEED_ANGULAR) + (errorAlfaAcomulado * KI_SPEED_ANGULAR) + (getLinearAceleration()* KD_SPEED_ANGULAR);  
//        
//
//       
//        
//        
//
//        moveLeft(map((constrain((REC_SPEED  - PIDAngularPosition), 0, TOP_SPEED)),0 ,TOP_SPEED, 0,255));
//        moveRight(map((constrain((REC_SPEED  + PIDAngularPosition), 0, TOP_SPEED)),0 ,TOP_SPEED, 0,255));
//        Serial.print(getwhereami_a());
//        Serial.print(errorAlfa);
//        Serial.print(alfaObjetivo);
//        Serial.println(PIDAngularPosition);
//
//        actualiza();
//      }
//      break;
  }

  switch (getwhereami_co ()) {//actualiza la posición y orientación en el laberinto
    case NORTE:
      setwhereami_co (2);
      setwhereami_cx (getwhereami_cx () + 1);
      break;
    case SUR:
      setwhereami_co (3);
      setwhereami_cx (getwhereami_cx () - 1);
      break;
    case ESTE:
      setwhereami_co (1);
      setwhereami_cy (getwhereami_cy () - 1);
      break;
    case OESTE:
      setwhereami_co (0);
      setwhereami_cy (getwhereami_cy () + 1);
      break;
  }
}

void giraIzquierda (){
  reseteaPosicion_xya(0.0,0.0,0.0);
  switch (getMode()) {
    case RECONOCIMIENTO:
      break;
    case CARRERA:
      break;
    case VUELTA_A_CASA:
      break;
    case TESTEO:
      actualiza();
      moveLeft(50); //70
      moveRight(70); //55
      delay(760);
      fullStop (); 
      delay (500);        
      break;
//    case VICTOR_TEST:
//      float errorSpeed = 0.0;
//      float alfaObjetivo = 1.5708; //corresponde a girar pi/2 hacia la izquierda los angulos en arduino están en radianes
//      float speedObjetivo = REC_SPEED;
//      float errorAlfa = 0.0;
//      float errorAlfaAcomulado = 0.0;
//      float PIDSpeed;
//      float PIDAngularPosition;
//      while (getwhereami_x ()< 90.0){
//
//        //se puede ir incrementan errorAlfa en las cuvas para tener una aceleración angular controlada
//        //esta aceleración puede ser inversamente proporcional a la velocidad que llevamos en ese momento, por lo que podemos no tener una aceleración lineal
//        // hasta que whereami_a+ errorAlfa sean igual o superior a alfaObjetivo, en ese momento ir a buscar el alfa objetivo, que este será angulo entre el vector del coche y el vector que forma coche con punto de salida de la curva
//        
//        //calculo errorAlfa para curvas a derecha
//        
//        errorSpeed =(REC_SPEED - getLinearSpeed());
//        errorAlfa = (alfaObjetivo - getwhereami_a());
//        errorAlfaAcomulado = errorAlfaAcomulado + errorAlfa;
//        PIDSpeed = (errorSpeed * KP_SPEED_LINEAR) + getLinearAceleration() * KD_SPEED_LINEAR ;
//        PIDAngularPosition = (errorAlfa * KP_SPEED_ANGULAR) + (errorAlfaAcomulado * KI_SPEED_ANGULAR) + (getLinearAceleration()* KD_SPEED_ANGULAR);  
//        
//
//       
//        
//        
//
//        moveLeft(map((constrain((REC_SPEED  - PIDAngularPosition), 0, TOP_SPEED)),0 ,TOP_SPEED, 0,255));
//        moveRight(map((constrain((REC_SPEED  + PIDAngularPosition), 0, TOP_SPEED)),0 ,TOP_SPEED, 0,255));
//        Serial.print(getwhereami_a());
//        Serial.print(errorAlfa);
//        Serial.print(alfaObjetivo);
//        Serial.println(PIDAngularPosition);
//
//        actualiza();
//      }
//      break;
   }
   
   switch (getwhereami_co ()) { //actualiza la posición y orientación en el laberinto
    case NORTE:
      setwhereami_co (3);
      setwhereami_cx (getwhereami_cx () - 1);
      break;
    case SUR:
      setwhereami_co (2);
      setwhereami_cx (getwhereami_cx () + 1);
      break;
    case ESTE:
      setwhereami_co (0);
      setwhereami_cy (getwhereami_cy () + 1);
      break;
    case OESTE:
      setwhereami_co (1);
      setwhereami_cy (getwhereami_cy () - 1);
      break;
   }
}

void avanzaDiagonalDerecha (byte N_casillas){
  reseteaPosicion_xya(0.0,0.0,0.0);
  switch (getMode()) {
    case RECONOCIMIENTO:
      // NOO, no tiene sentido usar diagonales en reconocimiento
      break;
    case CARRERA:
      // ideal, girar 45º acelerar decelerar y posicionarse en su casilla
      break;
    case VUELTA_A_CASA:
      //ir alternando giroderecha giro izquierda
      break;
    case TESTEO:
      byte casillas_recorridas = 0;
      while (casillas_recorridas < N_casillas){
        if (casillas_recorridas%2 == 0){
          giraDerecha ();
        }else{
          giraIzquierda (); 
        }
        casillas_recorridas++;
      }
      break;
//    case VICTOR_TEST:
//      break;
  }
}

void avanzaDiagonalIzquierda (byte N_casillas){
  reseteaPosicion_xya(0.0,0.0,0.0);
  switch (getMode()) {
    case RECONOCIMIENTO:
      // NOO, no tiene sentido usar diagonales en reconocimiento
      break;
    case CARRERA:
      // ideal, girar 45º acelerar decelerar y posicionarse en su casilla
      break;
    case VUELTA_A_CASA:
      //ir alternando giroderecha giro izquierda
      break;
    case TESTEO:
      byte casillas_recorridas = 0;
      while (casillas_recorridas < N_casillas){
        if (casillas_recorridas%2 == 0){
          giraIzquierda ();
        }else{
          giraDerecha (); 
        }
        casillas_recorridas++;
      }
      break;
//    case VICTOR_TEST:
//      break;
  }
}

void gira180 (){
  reseteaPosicion_xya(0.0,0.0,0.0);
  switch (getMode()) {
    case RECONOCIMIENTO:
      break;
    case CARRERA:
      break;
    case VUELTA_A_CASA:
      break;
    case TESTEO:
      moveLeft(50); //70
      moveRight(-70); //55
      delay(491);
      fullStop (); 
      delay (500);        
      break;
//    case VICTOR_TEST:
//      break;
  }
  switch (getwhereami_co ()) { //actualiza la posición y orientación en el laberinto
    case NORTE:
      setwhereami_co (SUR);
      setwhereami_cy (getwhereami_cy () - 1);
      break;
    case SUR:
      setwhereami_co (NORTE);
      setwhereami_cy (getwhereami_cy () + 1);
      break;
    case ESTE:
      setwhereami_co (OESTE);
      setwhereami_cx (getwhereami_cx () - 1);
      break;
    case OESTE:
      setwhereami_co (ESTE);
      setwhereami_cx (getwhereami_cx () + 1);
      break;
  }  
}
