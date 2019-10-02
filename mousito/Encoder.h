/*
Clase para ayudar a usar los encoders. Debido a las restricciones de las interrupciones, se debe
declarar una función en el sketch principal para que se pueda usar correctamente.
Ejemplo de uso:

1.- Declarar la variable global "Encoder *nombre;"
2.- En la función Setup() crear la instancia con:
    - nombre = new Encoder(pinA, pinB, &callback); // el pinA será el que se asigne a la interrupción
3.- En el archivo principal crear la función void callback (void) que llame al método signalReceived:
    - void callback (void) {nombre->signalReceived();}
4.- Cada encoder debe tener su propia función.

Una vez creada la clase y la función de callback los métodos a usar son:

  - getPulses(): Devuleve los pulsos totales de este encoder
  - savePulses(): Guarda los pulsos actuales para poder realizar comparaciones
  - getLastPulses(): Devuelve los pulsos que había la última vez que se llamó a savePulses();
*/

#ifndef encoder_h
  #define encoder_h

  class Encoder{
    public:
      Encoder(const uint8_t pinA, const uint8_t pinB, void(* callback)(void));
      void signalReceived();
      long getPulses();
      long getLastPulses();
      void savePulses();
    private:
      volatile long _pulses = 0;
      long _lastPulses = 0;
      uint8_t _pinA, _pinB;
  };
#endif