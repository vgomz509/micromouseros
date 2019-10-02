/*
Clase que se encargará del movimiento de los motores y el posicionamiento dentro
de la casilla a medida que avanza. NO se encargará de saber en qué casilla está
*/
#ifndef Motores_h
  #define Motores_h

  #include "Sensores.h"
  #include "Encoder.h"

  class Motores{
    public:
      Motores(byte mode, Sensores *sensors, Encoder *encoderLeft, Encoder *encoderRight);
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
      void moveLeftMotor (int speed);
      void moveRightMotor (int speed);
      Sensores *mySensors;
      Encoder *myEncoderLeft, *myEncoderRight;
      double myX, myY, myAlpha;
      double lastX, lastY, lastAlpha;
      double myLinearSpeed, myAngularSpeed, myLinearAccel, myAngularAccel;
      unsigned long lastStoredTime;
      byte myMode;
  };
#endif