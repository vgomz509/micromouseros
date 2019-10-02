/*
Clase que se encargará del movimiento de los motores y el posicionamiento dentro
de la casilla a medida que avanza. NO se encargará de saber en qué casilla está.
Al crear el objeto hay que pasarle los parámetros de modo, los pines para controlar
los motores, un puntero al objeto de sensores y dos punteros para los dos encoders.
*/
#ifndef Motores_h
  #define Motores_h

  #include "Sensores.h"
  #include "Encoder.h"

  struct PinesMotores{
    uint8_t leftForward, leftReverse, rightForward, rightReverse;
  };

  class Motores{
    public:
      Motores(byte mode,
              PinesMotores pinesMotores,
              Sensores *sensors,
              Encoder *encoderLeft,
              Encoder *encoderRight);
      void avanza (byte casillas);
      void avanzaDiagonalDerecha (byte casillas);
      void avanzaDiagonalIzquierda (byte casillas);
      void giraDerecha ();
      void giraIzquierda ();
      void gira180 ();
      void fullStop ();
      void setMode(byte newMode);
    private:
      void actualiza();
      void resetToZero();
      void moveLeftMotor (int speed);
      void moveRightMotor (int speed);
      Sensores *mySensors;
      Encoder *myEncoderLeft, *myEncoderRight;
      double myX, myY, myAlpha;
      double lastX, lastY, lastAlpha;
      double myLinearSpeed, myAngularSpeed, myLinearAccel, myAngularAccel;
      unsigned long lastStoredTime;
      byte myMode;
      PinesMotores myPines;
  };
#endif