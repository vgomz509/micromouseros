#ifndef status_control_h
#define status_control_h

// seters
void setwhereami_cx (byte newcx);
void setwhereami_cy (byte newcy);
void setwhereami_co (byte newco);
void doEncodeLeft();
void doEncodeRight();
void setMode(byte newMode);  
void reseteaPosicion_xya(float x, float y, float a);
void actualiza();


//geters
boolean hayParedIzquierda ();
boolean hayParedDelante ();
boolean hayParedDerecha ();
byte getwhereami_cx ();
byte getwhereami_cy ();
byte getwhereami_co ();
float getwhereami_x ();
float getwhereami_y ();
float getwhereami_a ();
float getLinearSpeed();
float getAngularSpeed();
float getLinearAceleration();

byte getMode();




#endif
