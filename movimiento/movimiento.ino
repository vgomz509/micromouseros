#include "Arduino.h"
#include <WiFi.h>
#include <WiFiMulti.h>
#include <ArduinoOTA.h>
#include "config.h"
#include "motion_control.h"
#include "status_control.h"
#include "display.h"
#include "U8g2lib.h"  //pantalla

Display *display;
TaskHandle_t otaServer;


void otaServerCode(void * parameters){
  WiFiMulti wifiMulti;

  wifiMulti.addAP("SORIANO", "9714207080");
  wifiMulti.addAP("HOME_SWEET_HOME", "M4ndr4k312345");
  // *****************************
  // Añadid vuestros AP aquí
  // *****************************
  
  while (wifiMulti.run() != WL_CONNECTED) {
    Serial.println("WiFi Connection Failed!");
    long startTime = millis();
    while (millis() - startTime < 5000){
      yield();
    }
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  ArduinoOTA.setHostname("mousito01");
  // Password can be set with it's md5 value as well
  // ArduinoOTA.setPasswordHash("6876bfa7143907ac8ec359181768bca5");

  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  for(;;){
    ArduinoOTA.handle();
    vTaskDelay(10);
  }
}

void mainCode(void * parameters){
  while(true){
    enviaDatosSerie();
  // setMode(display->selectMode());
  
    setMode(TESTEO);
    avanza(2);
    fullStop ();
    display->showPosition();
    delay (2000);
  }
}

void setup()
{
  pinMode(0, INPUT);
  pinMode(LEFT_ENCODER_A_PIN, INPUT_PULLUP);
  pinMode(RIGHT_ENCODER_A_PIN, INPUT_PULLUP);
  pinMode(LEFT_ENCODER_B_PIN, INPUT_PULLUP);
  pinMode(RIGHT_ENCODER_B_PIN, INPUT_PULLUP);

  ledcSetup(LEFT_MOTOR_REV_CH, FRECUENCIA_PWM, RESOLUCION_N_BITS_PWM);
  ledcSetup(LEFT_MOTOR_FWD_CH, FRECUENCIA_PWM, RESOLUCION_N_BITS_PWM);
  ledcSetup(RIGHT_MOTOR_REV_CH, FRECUENCIA_PWM, RESOLUCION_N_BITS_PWM);
  ledcSetup(RIGHT_MOTOR_FWD_CH, FRECUENCIA_PWM, RESOLUCION_N_BITS_PWM);
  
  ledcAttachPin(LEFT_MOTOR_REV_PIN, LEFT_MOTOR_REV_CH);
  ledcAttachPin(LEFT_MOTOR_FWD_PIN, LEFT_MOTOR_FWD_CH);
  ledcAttachPin(RIGHT_MOTOR_REV_PIN, RIGHT_MOTOR_REV_CH);
  ledcAttachPin(RIGHT_MOTOR_FWD_PIN, RIGHT_MOTOR_FWD_CH);
  
  Serial.begin(BAUD_RATE);
  attachInterrupt(LEFT_ENCODER_A_PIN, doEncodeLeft, CHANGE);
  attachInterrupt(RIGHT_ENCODER_A_PIN, doEncodeRight, CHANGE);
  // u8g2.begin();
  // initializeDisplay();
  display = new Display(/* rotation=*/U8G2_R0, /* clock=*/ OLED_SCL, /* data=*/ OLED_SDA, /* reset=*/ OLED_RST);

  xTaskCreatePinnedToCore(
    otaServerCode,
    "otaServer",
    10000,
    NULL,
    1,
    &otaServer,
    0);

  delay(500);  // needed to start-up otaServer
 }

void enviaDatosSerie() {
  actualiza();
  Serial.print(getwhereami_x());
  Serial.print (";");
  Serial.print(getwhereami_y());
  Serial.print (";");
  Serial.println(getwhereami_a());
  Serial.println(xPortGetCoreID());
}

void loop()
{
  //delay(1000);
  // enviaDatosSerie();
  // setMode(display->selectMode());

  setMode(TESTEO);
  avanza(2);
  fullStop ();
  display->showPosition();

  delay(2000);

}
