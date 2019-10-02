#include <Arduino.h>
#include "Encoder.h"

Encoder::Encoder (const uint8_t pinA, const uint8_t pinB, void(* callback)(void)){
  _pinA = pinA;
  _pinB = pinB;
  pinMode(_pinA, INPUT_PULLUP);
  pinMode(_pinB, INPUT_PULLUP);
  attachInterrupt(_pinA, callback, CHANGE);
}

void Encoder::signalReceived(){
  // if (digitalRead(_pinA) == digitalRead(_pinB)) pulses++;
  // else pulses--;
  digitalRead(_pinA) == digitalRead(_pinB) ? _pulses++ : _pulses--;   // Si el pinA es igual al pinB aumenta los pulsos, en caso contrario los decrementa.
}

long Encoder::getPulses(){
  return _pulses;
}

long Encoder::getLastPulses(){
  return _lastPulses;
}

void Encoder::savePulses(){
  _lastPulses = _pulses;
}