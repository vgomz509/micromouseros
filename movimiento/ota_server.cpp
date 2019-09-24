#include <WiFi.h>
#include <WiFiMulti.h>
#include <ArduinoOTA.h>

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
      vTaskDelay(10);
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

void createOTAServer(TaskHandle_t *otaServer){
  xTaskCreatePinnedToCore(
      otaServerCode,
      "otaServer",
      10000,
      NULL,
      1,
      otaServer,
      0);

  delay(500);  // needed to start-up otaServer
}

String getIP(){
  return WiFi.localIP().toString();
}