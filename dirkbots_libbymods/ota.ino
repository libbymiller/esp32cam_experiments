void ota_setup() {

  // Port defaults to 3232
  // ArduinoOTA.setPort(3232);

  // Hostname defaults to esp3232-[MAC]

  // No authentication by default
  //ArduinoOTA.setPassword("");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA
  .onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Log.println("Start updating " + type);
  })
  .onEnd([]() {
    Log.println("\nEnd");
  })
  .onProgress([](unsigned int progress, unsigned int total) {
    Log.printf("Progress: %u%%\r", (progress / (total / 100)));
  })
  .onError([](ota_error_t error) {
    Log.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Log.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Log.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Log.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Log.println("Receive Failed");
    else if (error == OTA_END_ERROR) Log.println("End Failed");
  });

  ArduinoOTA.begin();
}

void ota_loop() {
  ArduinoOTA.handle();
}
