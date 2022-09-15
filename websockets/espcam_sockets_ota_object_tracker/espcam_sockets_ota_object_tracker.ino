/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-cam-video-streaming-web-server-camera-home-assistant/
  
  IMPORTANT!!! 
   - Select Board "AI Thinker ESP32-CAM"
   - GPIO 0 must be connected to GND to upload a sketch
   - After connecting GPIO 0 to GND, press the ESP32-CAM on-board RESET button to put your board in flashing mode
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/
//worked with https://stackoverflow.com/questions/50620821/uint8array-to-image-in-javascript

#include "esp_camera.h"
#include <WiFi.h>
#include "esp_timer.h"
#include "img_converters.h"
#include "Arduino.h"
#include "fb_gfx.h"
#include "soc/soc.h" //disable brownout problems
#include "soc/rtc_cntl_reg.h"  //disable brownout problems
#include "esp_http_server.h"
#include <WebSocketsClient.h>

#include <ArduinoOTA.h>

#include <Servo.h>

Servo servo_pan;  
Servo servo_tilt; 

//servo position
int pos_pan = 90;    
int pos_tilt = 90;    

// Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33 
// you have to use a timer higher than 2, see https://www.esp32.com/viewtopic.php?t=11379
int servoPin_pan = 14; 
int servoPin_tilt = 2; 


//Replace with your network credentials
const char* ssid = "*****";
const char* password = "*****";
WebSocketsClient webSocket; // websocket client class instance

#define PART_BOUNDARY "123456789000000000000987654321"

#define CAMERA_MODEL_AI_THINKER


#if defined(CAMERA_MODEL_AI_THINKER)
  #define PWDN_GPIO_NUM     32
  #define RESET_GPIO_NUM    -1
  #define XCLK_GPIO_NUM      0
  #define SIOD_GPIO_NUM     26
  #define SIOC_GPIO_NUM     27
  
  #define Y9_GPIO_NUM       35
  #define Y8_GPIO_NUM       34
  #define Y7_GPIO_NUM       39
  #define Y6_GPIO_NUM       36
  #define Y5_GPIO_NUM       21
  #define Y4_GPIO_NUM       19
  #define Y3_GPIO_NUM       18
  #define Y2_GPIO_NUM        5
  #define VSYNC_GPIO_NUM    25
  #define HREF_GPIO_NUM     23
  #define PCLK_GPIO_NUM     22
#else
  #error "Camera model not selected"
#endif


void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
 
  Serial.begin(115200);

  Serial.println("starting");
  esp_log_level_set("*", ESP_LOG_VERBOSE);
  pinMode(servoPin_pan, OUTPUT);
  pinMode(servoPin_tilt, OUTPUT);

  //https://stackoverflow.com/questions/64402915/esp32-cam-with-servo-control-wont-work-arduino-ide
  //and
  //https://www.esp32.com/viewtopic.php?t=11379
  servo_pan.attach(servoPin_pan, 4); 
  servo_tilt.attach(servoPin_tilt, 5); 

  servo_pan.write(90);
  servo_tilt.write(90);
  
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  //config.pixel_format = PIXFORMAT_JPEG; 
  config.pixel_format = PIXFORMAT_GRAYSCALE; 

  
  if(psramFound()){
      Serial.println("PSRAM found. Setting up camera with PSRAM");
//    config.frame_size = FRAMESIZE_HVGA;
    config.frame_size = FRAMESIZE_QQVGA;

    config.jpeg_quality = 40;
    config.fb_count = 1;
  } else {
 //   config.frame_size = FRAMESIZE_HVGA;
    config.frame_size = FRAMESIZE_QQVGA;

    config.jpeg_quality = 20;
    config.fb_count = 2;
  }
  
  // Camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
  sensor_t * s = esp_camera_sensor_get();
  s->set_vflip(s, 1);
  
  // Wi-Fi connection
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());

  // connect to server
  Serial.println("starting WS Connection");
  webSocket.begin("192.168.1.18", 80, "/", "");
  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(5000);
  ota_setup();
}

void loop() {
  webSocket.loop();
  
  delay(1);
  
}
