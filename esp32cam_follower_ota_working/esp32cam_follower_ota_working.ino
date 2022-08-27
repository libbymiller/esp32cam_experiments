#include <Servo.h>

#include <esp_camera.h>
#include <WiFi.h>
#include "esp_http_server.h"
#include <ArduinoOTA.h>
#include <ESPmDNS.h>

Servo servo_pan;  
Servo servo_tilt; 

//servo position
int pos_pan = 90;    
int pos_tilt = 90;    

// Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33 
// you have to use a timer higher than 2, see https://www.esp32.com/viewtopic.php?t=11379
int servoPin_pan = 14; 
int servoPin_tilt = 2; 

// Change YOUR_AP_NAME and YOUR_AP_PASSWORD to your WiFi credentials
const char *ssid = "XXXXXXX";       // Put your SSID here
const char *password = "XXXXXXX"; // Put your PASSWORD here

// AI Thinker
// https://github.com/SeeedDocument/forum_doc/raw/master/reg/ESP32_CAM_V1.6.pdf

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
#define LED_PIN           33 // Status led
#define LED_ON           LOW // - Pin is inverted.
#define LED_OFF         HIGH //
#define LAMP_PIN           4 // LED FloodLamp.

static camera_config_t camera_config = {
    .pin_pwdn = PWDN_GPIO_NUM,
    .pin_reset = RESET_GPIO_NUM,
    .pin_xclk = XCLK_GPIO_NUM,
    .pin_sscb_sda = SIOD_GPIO_NUM,
    .pin_sscb_scl = SIOC_GPIO_NUM,
    .pin_d7 = Y9_GPIO_NUM,
    .pin_d6 = Y8_GPIO_NUM,
    .pin_d5 = Y7_GPIO_NUM,
    .pin_d4 = Y6_GPIO_NUM,
    .pin_d3 = Y5_GPIO_NUM,
    .pin_d2 = Y4_GPIO_NUM,
    .pin_d1 = Y3_GPIO_NUM,

    .pin_d0 = Y2_GPIO_NUM,
    .pin_vsync = VSYNC_GPIO_NUM,
    .pin_href = HREF_GPIO_NUM,
    .pin_pclk = PCLK_GPIO_NUM,
    .xclk_freq_hz = 20000000,

    .ledc_timer = LEDC_TIMER_0,

    .ledc_channel = LEDC_CHANNEL_0,

    //https://forum.arduino.cc/t/esp32_cam-acces-and-process-image/677628/6
    //"you get 19200 bytes with is correct for 160x120 grayscale pixels on 8 bits"
    .pixel_format = PIXFORMAT_GRAYSCALE,//YUV422,GRAYSCALE,RGB565,JPEG
    .frame_size = FRAMESIZE_QQVGA,//QQVGA-QXGA Do not use sizes above QVGA when not JPEG
    .jpeg_quality = 10, //0-63 lower number means higher quality
    .fb_count = 1 //if more than one, i2s runs in continuous mode. Use only with JPEG
    //.grab_mode = CAMERA_GRAB_WHEN_EMPTY//CAMERA_GRAB_LATEST. Sets when buffers should be filled [doesn't work]
};

esp_err_t camera_init(){
    //power up the camera if PWDN pin is defined
    if(PWDN_GPIO_NUM != -1){
        pinMode(PWDN_GPIO_NUM, OUTPUT);
        digitalWrite(PWDN_GPIO_NUM, LOW);
    }

    //initialize the camera
    esp_err_t err = esp_camera_init(&camera_config);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Camera Init Failed");
        return err;
    }

    Serial.println("Cameria initialised");
    sensor_t * s = esp_camera_sensor_get();
    s->set_vflip(s, 1);
    return ESP_OK;
}


void setup() {

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
    
    Serial.begin(115200);

    camera_init();


    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
   
    Serial.println("WiFi connected..!");
    delay(100);
    
    Serial.print("Camera Stream Ready! Go to: http://");
    Serial.print(WiFi.localIP());

    // Start streaming web server
    startCameraServer();
    ota_setup();
}

void loop(){
    ArduinoOTA.handle();
}
