/* Clase para ayudar a imprimir un menú por pantalla en el TTGO LoRa32-OLED V1
Se debe inicializar el objeto en el constructor con los parámetros correctos
La rotación actualmente sólo funciona en vertical (U8G2_R0)
Ejemplo de uso:

Display *display; // Para tener la variable global

// En el setup se inicializa el display
display = new Display(U8G2_R0, OLED_SCL, OLED_SDA, OLED_RST);

// Es necesario un String[] con el menú y saber el número de elementos que tendrá
// para pasarlos a la función:
String menu[]={"Reconocimiento", "Carrera", "Vuelta a casa", "Testeo", "Juanjo", "Sergio", "Victor"};
int elegido = display->selectFromMenu(menu, 7);
// La función selectFromMenu devolverá la posición elegida (empezando el 0)*/

#ifndef display_h
  #define display_h
  #include "U8g2lib.h"  //pantalla
  #ifdef U8X8_HAVE_HW_SPI
    #include <SPI.h>
  #endif
  #ifdef U8X8_HAVE_HW_I2C
    #include <Wire.h>
  #endif

  class Display
  {
    public:
      Display(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset);
      void showPosition();
      void showIP(String address);
      uint8_t selectFromMenu(String menu[], uint8_t elements);
    private:
      U8G2_SSD1306_128X64_NONAME_F_SW_I2C *_u8g2;
      void printMenu(String [], uint8_t, uint8_t);
  };
#endif
