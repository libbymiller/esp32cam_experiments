#include <esp_camera.h>
#include <WiFi.h>
#include "esp_http_server.h"
#include <ArduinoOTA.h>
#include <ESPmDNS.h>
#include <Servo.h>
Servo servo_pan;
Servo servo_tilt;


// Uncomment the next line if you want a Access Point
#define WIFI_AP_MODE

const char APNAME[32] = "legobot-libby";

#include <TLog.h>      // The T-Logging library.
// Run a telnet service on the default port (23) which shows what is
// sent to Serial if you telnet to it.
//
#include <TelnetSerialStream.h>
TelnetSerialStream telnetSerialStream = TelnetSerialStream();

// Neutral position - in degrees
const float NEUTRAL_POS_X = 90;
const float NEUTRAL_POS_Y = 125;

// (Initial) servo position - in degrees
int pos_pan = NEUTRAL_POS_X;
int pos_tilt = NEUTRAL_POS_Y;

const float CENTER_CAM_X = 0.5;
const float CENTER_CAM_Y = 0.5;

// servos - how fast they travel; i.e. how
// long to wait (at least) between each update
// of one degree.
const  int wait = 15; // mSeconds *

#ifndef WIFI_NETWORK
#define WIFI_NETWORK "XXXXX"
#endif

#ifndef WIFI_PASSWD
#define WIFI_PASSWD "XXXXX"
#endif

// Change YOUR_AP_NAME and YOUR_AP_PASSWORD to your WiFi credentials
const char *ssid = WIFI_NETWORK;       // Put your SSID here
const char *password = WIFI_PASSWD; // Put your PASSWORD here

// Postition of the server - remembered.
//
float m_attentionX = NEUTRAL_POS_X;
float m_attentionY = NEUTRAL_POS_Y;


void setup() {
  Serial.begin(115200);
  esp_log_level_set("*", ESP_LOG_VERBOSE);


  setup_camera();
  setup_servo();

#ifdef WIFI_AP_MODE

  WiFi.softAP(APNAME,NULL,5);
  IPAddress myIP = WiFi.softAPIP();
  Log.printf("WiFi station broadcaasting on <%s>\n", APNAME);
#else
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Log.print(".");
  }
  Log.println("WiFi connected..!");
  IPAddress myIP =WiFi.localIP();

#endif

  Log.addPrintStream(std::make_shared<TelnetSerialStream>(telnetSerialStream));
  Log.begin();
  Log.println("starting");
  
  delay(100);

  MDNS.begin(APNAME);

  Log.print("Camera Stream Ready! Go to: http://");
  Log.println(myIP);

  Log.printf("Heap: Size: %.1f kB, Free: %.1f kB,, Min free: %.1f kB, Max Alloc: %.1f kB\n",
             ESP.getHeapSize() / 1024.,
             ESP.getFreeHeap() / 1024.,
             ESP.getMinFreeHeap() / 1024.,
             ESP.getMaxAllocHeap() / 1024.
            );

  // Start streaming web server
  startCameraServer();
  ota_setup();
  setup_pid();
  setup_report_memory();

}

void camera_and_movement_loop() {
  static unsigned long ok = 0, fail = 0;

  camera_fb_t * current_frame = esp_camera_fb_get();
  if (!current_frame)  {
    static unsigned long last_error = 0;
    fail++;
    if (last_error == 0 || millis() - last_error > 2000) {
      Log.printf("Camera capture failed ok=%lu/fail=%lu\n", ok, fail);
      last_error = millis();
    };
    return;
  };
  ok++;


  // diff() will also update previous_frame with some accomodation of
  // slowly changing light conditions.
  //
  camera_fb_t * difference = diff(current_frame);
  esp_camera_fb_return(current_frame);

  if (difference == NULL)
    return; // no difference yet - is the first image captured.


  float cogX = 0, cogY = 0; // center of gravity

  // Calculate the center of gravity.
  //
  int perc = calculate_cog(difference, &cogX, &cogY);

  // draw a crosshair at the found CoG.
  if (1) {
    int pX = cogX *  difference->width;
    int pY = difference->height - cogY *  difference->height;

    for (int x =  0; x <  difference->width; x++)
      difference->buf[x + pY * difference->width] = 128;
    for (int y =  0; y <  difference->height; y++)
      difference->buf[pX + y * difference->width] = 128;
  };

  stream_next_image(difference);

  if (perc < 5) {
    // low confidence in what we see - let the servos move slowy to the center.
    //
    m_attentionX = m_attentionX + (((NEUTRAL_POS_X - m_attentionX)) / 20);
    m_attentionY = m_attentionY + (((NEUTRAL_POS_Y - m_attentionY)) / 20);

    pan((int)pos_pan, (int)(m_attentionX), wait);
    tilt((int)pos_tilt, (int)(m_attentionY), wait);
    return;
  }

  // Camera is mounted fixed; pan and tilt above it and
  // with roughly same coordinate system.
  //
  move_straight_away(cogX, cogY);

  // Camera is mounted on the pan and tilt. We move the camera
  // when we move pan and tilt.
  //
  //move_relative(cogX, cogY);

  // Use a PID controller.
  //
  //move_pid(cogX, cogY);
}


void loop() {
  Log.loop();
  ota_loop();
  camera_and_movement_loop();
  loop_report_memory();
  loop_pid();
}
