//static const char* _STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
//static const char* _STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char* _STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";

httpd_handle_t stream_httpd = NULL;

static esp_err_t stream_handler(){
  Serial.println("Starting stream.");
  camera_fb_t * fb = NULL;
  esp_err_t res = ESP_OK;
  size_t _jpg_buf_len = 0;
  uint8_t * _jpg_buf = NULL;
  char * part_buf[64];

  while(true){
    ArduinoOTA.handle();//??here?
    fb = esp_camera_fb_get();
    if (!fb) {
      esp_camera_fb_return(fb);
      Serial.println("Camera capture failed");
      res = ESP_FAIL;
    } else {

      //if(fb->width > 10){
        if(fb->format != PIXFORMAT_JPEG){
          camera_fb_t * rf =  diff(fb); 
          bool jpeg_converted = frame2jpg(rf, 80, &_jpg_buf, &_jpg_buf_len);
          webSocket.sendBIN(_jpg_buf, _jpg_buf_len);
          esp_camera_fb_return(fb);
          fb = NULL;
          if(!jpeg_converted){
            Serial.println("JPEG compression failed");
            res = ESP_FAIL;
          }
        } else {
          _jpg_buf_len = fb->len;
          _jpg_buf = fb->buf;
        }
      //}
    
    }
    if(res == ESP_OK){
      size_t hlen = snprintf((char *)part_buf, 64, _STREAM_PART, _jpg_buf_len);
      Serial.printf("Streaming %u bytes\n", _jpg_buf_len);
      webSocket.sendBIN(_jpg_buf, _jpg_buf_len);
    }
    if(fb){
      esp_camera_fb_return(fb);
      fb = NULL;
      _jpg_buf = NULL;
    } else if(_jpg_buf){
      //Serial.println("Freeing _jpg_buf");
      free(_jpg_buf);
      _jpg_buf = NULL;
    }
    if(res != ESP_OK){
      //hmm break;
    }
    // Serial.printf("MJPG: %uB\n",(uint32_t)(_jpg_buf_len));
  }
  return res;
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
      case WStype_DISCONNECTED:
        Serial.printf("[WSc] Disconnected!\n");
        // connected = false;
        break;
      case WStype_CONNECTED: {
        Serial.printf("[WSc] Connected to url: %s\n", payload);
        // connected = true;
        stream_handler();
        // send message to server when Connected
        Serial.println("[WSc] SENT: Connected");
        webSocket.sendTXT("Connected");
      }
        break;
  }
}
