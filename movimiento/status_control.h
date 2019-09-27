#ifndef status_control_h
#define status_control_h

// seters
void setwhereami_cx (uint8_t newcx);
void setwhereami_cy (uint8_t newcy);
void setwhereami_co (uint8_t newco);
void doEncodeLeft();
void doEncodeRight();
void setMode(uint8_t newMode);  
void reseteaPosicion_xya(double x, double y, double a);
void actualiza();
void actualizaParedes();


//geters
boolean hayParedIzquierda ();
boolean hayParedDelante ();
boolean hayParedDerecha ();
uint8_t getwhereami_cx ();
uint8_t getwhereami_cy ();
uint8_t getwhereami_co ();
double getwhereami_x ();
double getwhereami_y ();
double getwhereami_a ();
float getLinearSpeed();
float getAngularSpeed();
float getLinearAceleration();
long getPulsesLeft ();
long getPulsesRight();


uint8_t getMode();




#endif
