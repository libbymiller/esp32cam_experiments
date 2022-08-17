#include <esp_camera.h>
#include <WiFi.h>
#include "esp_http_server.h"


// Change YOUR_AP_NAME and YOUR_AP_PASSWORD to your WiFi credentials
const char *ssid = "XXXX";       // Put your SSID here
const char *password = "XXXX"; // Put your PASSWORD here


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
    //.pixel_format = PIXFORMAT_JPEG,
    .frame_size = FRAMESIZE_QQVGA,//QQVGA-QXGA Do not use sizes above QVGA when not JPEG

    .jpeg_quality = 10, //0-63 lower number means higher quality
    .fb_count = 1 //if more than one, i2s runs in continuous mode. Use only with JPEG
    //.grab_mode = CAMERA_GRAB_WHEN_EMPTY//CAMERA_GRAB_LATEST. Sets when buffers should be filled. doesn't work
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

    Serial.println("ok I guess");
    return ESP_OK;
}


camera_fb_t * resultFrame = new camera_fb_t();

uint8_t * img_buf_bg = NULL; 

camera_fb_t * diff(camera_fb_t * fb){
   Serial.println("diffing");
   //first run
   if(!img_buf_bg){
       //Serial.println("returning!");
       //img_buf_bg = fb->buf;
       img_buf_bg = new uint8_t[19200];
       memcpy(img_buf_bg, fb->buf, 19200);
       return resultFrame;    
   }

   //compare bufs
   int bitsPerPixel = 8; //8 bit greyscale
   int numPixels = (8 * fb->len) / bitsPerPixel; //stolen from Richard, cancles out!
   uint16_t delta = 40; //Richard's is more complex
   uint32_t sumX = 0;
   uint32_t sumY = 0;
   uint32_t sumN = 0;
   uint16_t x = 0, y = 0;

   //just make a copy to use, we replace all the pixels anyhoo
   memcpy(&resultFrame, &fb, sizeof fb);

   for( uint32_t i = 0; i < numPixels; i += 1 ){
        x ++;
        if( x >= fb->width ){
           y++;
           x = 0;
        }

        uint16_t pb = fb->buf[ i ];
        uint16_t pf = img_buf_bg[ i ];

        if( ( pf > pb && pf - pb > delta ) || ( pf < pb && pb - pf > delta )) {
            // different from background
            resultFrame->buf[ i ] = (uint8_t) 255;//???

            sumX += x;
            sumY += y;
            sumN ++;
         } else {
            // same-ish as background
            resultFrame->buf[ i ] = (uint8_t) 0;
         }
   }
   
   if( fb->len < 1 ){
       fb->len = 1;
   }
   uint32_t percentage = (sumN * 100) / fb->len;
   Serial.print("\r\n%d percent changed pixels: ");
   Serial.println(percentage);
   Serial.println(sumN);
   Serial.println(resultFrame->len);

   //set background to fb
   memcpy(img_buf_bg, fb->buf, 19200);

   return resultFrame;

}



//from https://www.electrorules.com/esp32-cam-video-streaming-web-server-works-with-home-assistant/
#define PART_BOUNDARY "123456789000000000000987654321"

static const char* _STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char* _STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char* _STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";
 
httpd_handle_t stream_httpd = NULL;
 
static esp_err_t stream_handler(httpd_req_t *req){
  camera_fb_t * fb = NULL;
  esp_err_t res = ESP_OK;
  size_t _jpg_buf_len = 0;
  uint8_t * _jpg_buf = NULL;
  char * part_buf[64];
 
  res = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);
  if(res != ESP_OK){
    return res;
  }
 
  while(true){
    fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Camera capture failed");
      res = ESP_FAIL;
    } else {
          camera_fb_t * rf = diff(fb);
          bool jpeg_converted = frame2jpg(rf, 80, &_jpg_buf, &_jpg_buf_len);
          ///esp_camera_fb_return(fb);
          fb = NULL;
          if(!jpeg_converted){
            Serial.println("JPEG compression failed");
            res = ESP_FAIL;
          }
    }
    if(res == ESP_OK){
      size_t hlen = snprintf((char *)part_buf, 64, _STREAM_PART, _jpg_buf_len);
      res = httpd_resp_send_chunk(req, (const char *)part_buf, hlen);
    }
    if(res == ESP_OK){
      res = httpd_resp_send_chunk(req, (const char *)_jpg_buf, _jpg_buf_len);
    }
    if(res == ESP_OK){
      res = httpd_resp_send_chunk(req, _STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY));
    }
    if(fb){
      esp_camera_fb_return(fb);
      fb = NULL;
      _jpg_buf = NULL;
    } else if(_jpg_buf){
      free(_jpg_buf);
      _jpg_buf = NULL;
    }
    if(res != ESP_OK){
      break;
    }
    //Serial.printf("MJPG: %uB\n",(uint32_t)(_jpg_buf_len));
  }
  return res;
}
 
void startCameraServer(){
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  config.server_port = 80;
 
  httpd_uri_t index_uri = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = stream_handler,
    .user_ctx  = NULL
  };
   
  //Serial.printf("Starting web server on port: '%d'\n", config.server_port);
  if (httpd_start(&stream_httpd, &config) == ESP_OK) {
    httpd_register_uri_handler(stream_httpd, &index_uri);
  }
}
 


void setup() {
    Serial.begin(115200);

    camera_init();

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
   
    Serial.println("WiFi connected..!");

    Serial.print("Camera Stream Ready! Go to: http://");
    Serial.print(WiFi.localIP());
   
    // Start streaming web server
    startCameraServer();

}

void loop(){
   //Serial.println("get ready");
   delay(100);
}
