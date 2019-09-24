#ifndef ota_server_h
  #define ota_server_h
  #include <WiFi.h>
  #include <WiFiMulti.h>
  #include <ArduinoOTA.h>

  void otaServerCode(void * parameters);
  void createOTAServer(TaskHandle_t *otaServer);
  String getIP();

#endif
