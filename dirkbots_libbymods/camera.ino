
// AI Thinker
// https://github.com/SeeedDocument/forum_doc/raw/master/reg/ESP32_CAM_V1.6.pdf
#define CAMERA_MODEL_AI_THINKER // Has PSRAM

#ifdef libby
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
#endif


#if defined(CAMERA_MODEL_WROVER_KIT)
#define PWDN_GPIO_NUM    -1
#define RESET_GPIO_NUM   -1
#define XCLK_GPIO_NUM    21
#define SIOD_GPIO_NUM    26
#define SIOC_GPIO_NUM    27

#define Y9_GPIO_NUM      35
#define Y8_GPIO_NUM      34
#define Y7_GPIO_NUM      39
#define Y6_GPIO_NUM      36
#define Y5_GPIO_NUM      19
#define Y4_GPIO_NUM      18
#define Y3_GPIO_NUM       5
#define Y2_GPIO_NUM       4
#define VSYNC_GPIO_NUM   25
#define HREF_GPIO_NUM    23
#define PCLK_GPIO_NUM    22

#elif defined(CAMERA_MODEL_ESP_EYE)
#define PWDN_GPIO_NUM    -1
#define RESET_GPIO_NUM   -1
#define XCLK_GPIO_NUM    4
#define SIOD_GPIO_NUM    18
#define SIOC_GPIO_NUM    23

#define Y9_GPIO_NUM      36
#define Y8_GPIO_NUM      37
#define Y7_GPIO_NUM      38
#define Y6_GPIO_NUM      39
#define Y5_GPIO_NUM      35
#define Y4_GPIO_NUM      14
#define Y3_GPIO_NUM      13
#define Y2_GPIO_NUM      34
#define VSYNC_GPIO_NUM   5
#define HREF_GPIO_NUM    27
#define PCLK_GPIO_NUM    25

#elif defined(CAMERA_MODEL_M5STACK_PSRAM)
#define PWDN_GPIO_NUM     -1
#define RESET_GPIO_NUM    15
#define XCLK_GPIO_NUM     27
#define SIOD_GPIO_NUM     25
#define SIOC_GPIO_NUM     23

#define Y9_GPIO_NUM       19
#define Y8_GPIO_NUM       36
#define Y7_GPIO_NUM       18
#define Y6_GPIO_NUM       39
#define Y5_GPIO_NUM        5
#define Y4_GPIO_NUM       34
#define Y3_GPIO_NUM       35
#define Y2_GPIO_NUM       32
#define VSYNC_GPIO_NUM    22
#define HREF_GPIO_NUM     26
#define PCLK_GPIO_NUM     21

#elif defined(CAMERA_MODEL_M5STACK_V2_PSRAM)
#define PWDN_GPIO_NUM     -1
#define RESET_GPIO_NUM    15
#define XCLK_GPIO_NUM     27
#define SIOD_GPIO_NUM     22
#define SIOC_GPIO_NUM     23

#define Y9_GPIO_NUM       19
#define Y8_GPIO_NUM       36
#define Y7_GPIO_NUM       18
#define Y6_GPIO_NUM       39
#define Y5_GPIO_NUM        5
#define Y4_GPIO_NUM       34
#define Y3_GPIO_NUM       35
#define Y2_GPIO_NUM       32
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     26
#define PCLK_GPIO_NUM     21

#elif defined(CAMERA_MODEL_M5STACK_WIDE)
#define PWDN_GPIO_NUM     -1
#define RESET_GPIO_NUM    15
#define XCLK_GPIO_NUM     27
#define SIOD_GPIO_NUM     22
#define SIOC_GPIO_NUM     23

#define Y9_GPIO_NUM       19
#define Y8_GPIO_NUM       36
#define Y7_GPIO_NUM       18
#define Y6_GPIO_NUM       39
#define Y5_GPIO_NUM        5
#define Y4_GPIO_NUM       34
#define Y3_GPIO_NUM       35
#define Y2_GPIO_NUM       32
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     26
#define PCLK_GPIO_NUM     21

#elif defined(CAMERA_MODEL_M5STACK_ESP32CAM)
#define PWDN_GPIO_NUM     -1
#define RESET_GPIO_NUM    15
#define XCLK_GPIO_NUM     27
#define SIOD_GPIO_NUM     25
#define SIOC_GPIO_NUM     23

#define Y9_GPIO_NUM       19
#define Y8_GPIO_NUM       36
#define Y7_GPIO_NUM       18
#define Y6_GPIO_NUM       39
#define Y5_GPIO_NUM        5
#define Y4_GPIO_NUM       34
#define Y3_GPIO_NUM       35
#define Y2_GPIO_NUM       17
#define VSYNC_GPIO_NUM    22
#define HREF_GPIO_NUM     26
#define PCLK_GPIO_NUM     21

#elif defined(CAMERA_MODEL_M5STACK_UNITCAM)
#define PWDN_GPIO_NUM     -1
#define RESET_GPIO_NUM    15
#define XCLK_GPIO_NUM     27
#define SIOD_GPIO_NUM     25
#define SIOC_GPIO_NUM     23

#define Y9_GPIO_NUM       19
#define Y8_GPIO_NUM       36
#define Y7_GPIO_NUM       18
#define Y6_GPIO_NUM       39
#define Y5_GPIO_NUM        5
#define Y4_GPIO_NUM       34
#define Y3_GPIO_NUM       35
#define Y2_GPIO_NUM       32
#define VSYNC_GPIO_NUM    22
#define HREF_GPIO_NUM     26
#define PCLK_GPIO_NUM     21

#elif defined(CAMERA_MODEL_AI_THINKER)
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

#elif defined(CAMERA_MODEL_TTGO_T_JOURNAL)
#define PWDN_GPIO_NUM      0
#define RESET_GPIO_NUM    15
#define XCLK_GPIO_NUM     27
#define SIOD_GPIO_NUM     25
#define SIOC_GPIO_NUM     23

#define Y9_GPIO_NUM       19
#define Y8_GPIO_NUM       36
#define Y7_GPIO_NUM       18
#define Y6_GPIO_NUM       39
#define Y5_GPIO_NUM        5
#define Y4_GPIO_NUM       34
#define Y3_GPIO_NUM       35
#define Y2_GPIO_NUM       17
#define VSYNC_GPIO_NUM    22
#define HREF_GPIO_NUM     26
#define PCLK_GPIO_NUM     21


#elif defined(CAMERA_MODEL_ESP32_CAM_BOARD)
// The 18 pin header on the board has Y5 and Y3 swapped
#define USE_BOARD_HEADER 0
#define PWDN_GPIO_NUM    32
#define RESET_GPIO_NUM   33
#define XCLK_GPIO_NUM     4
#define SIOD_GPIO_NUM    18
#define SIOC_GPIO_NUM    23

#define Y9_GPIO_NUM      36
#define Y8_GPIO_NUM      19
#define Y7_GPIO_NUM      21
#define Y6_GPIO_NUM      39
#if USE_BOARD_HEADER
#define Y5_GPIO_NUM      13
#else
#define Y5_GPIO_NUM      35
#endif
#define Y4_GPIO_NUM      14
#if USE_BOARD_HEADER
#define Y3_GPIO_NUM      35
#else
#define Y3_GPIO_NUM      13
#endif
#define Y2_GPIO_NUM      34
#define VSYNC_GPIO_NUM    5
#define HREF_GPIO_NUM    27
#define PCLK_GPIO_NUM    25

#elif defined(CAMERA_MODEL_ESP32S3_CAM_LCD)
#define PWDN_GPIO_NUM     -1
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM     40
#define SIOD_GPIO_NUM     17
#define SIOC_GPIO_NUM     18

#define Y9_GPIO_NUM       39
#define Y8_GPIO_NUM       41
#define Y7_GPIO_NUM       42
#define Y6_GPIO_NUM       12
#define Y5_GPIO_NUM       3
#define Y4_GPIO_NUM       14
#define Y3_GPIO_NUM       47
#define Y2_GPIO_NUM       13
#define VSYNC_GPIO_NUM    21
#define HREF_GPIO_NUM     38
#define PCLK_GPIO_NUM     11

#elif defined(CAMERA_MODEL_ESP32S2_CAM_BOARD)
// The 18 pin header on the board has Y5 and Y3 swapped
#define USE_BOARD_HEADER 0
#define PWDN_GPIO_NUM     1
#define RESET_GPIO_NUM    2
#define XCLK_GPIO_NUM     42
#define SIOD_GPIO_NUM     41
#define SIOC_GPIO_NUM     18

#define Y9_GPIO_NUM       16
#define Y8_GPIO_NUM       39
#define Y7_GPIO_NUM       40
#define Y6_GPIO_NUM       15
#if USE_BOARD_HEADER
#define Y5_GPIO_NUM       12
#else
#define Y5_GPIO_NUM       13
#endif
#define Y4_GPIO_NUM       5
#if USE_BOARD_HEADER
#define Y3_GPIO_NUM       13
#else
#define Y3_GPIO_NUM       12
#endif
#define Y2_GPIO_NUM       14
#define VSYNC_GPIO_NUM    38
#define HREF_GPIO_NUM     4
#define PCLK_GPIO_NUM     3

#elif defined(CAMERA_MODEL_ESP32S3_EYE)
#define PWDN_GPIO_NUM -1
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 15
#define SIOD_GPIO_NUM 4
#define SIOC_GPIO_NUM 5

#define Y2_GPIO_NUM 11
#define Y3_GPIO_NUM 9
#define Y4_GPIO_NUM 8
#define Y5_GPIO_NUM 10
#define Y6_GPIO_NUM 12
#define Y7_GPIO_NUM 18
#define Y8_GPIO_NUM 17
#define Y9_GPIO_NUM 16

#define VSYNC_GPIO_NUM 6
#define HREF_GPIO_NUM 7
#define PCLK_GPIO_NUM 13

#else
#error "Camera model not selected"
#endif

#if libby
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
  .fb_count = 1, //if more than one, i2s runs in continuous mode. Use only with JPEG
#if !defined(CAMERA_FB_IN_PSRAM) || !defined(CAMERA_FB_IN_DRAM)
  .fb_location = CAMERA_FB_IN_PSRAM // XXX added
#endif
#ifdef CAMERA_GRAB_WHEN_EMPTY
  .grab_mode = CAMERA_GRAB_WHEN_EMPTY // XXX added
#endif
  };
#endif

esp_err_t setup_camera() {
  pinMode(PWDN_GPIO_NUM, OUTPUT);
  digitalWrite(PWDN_GPIO_NUM, LOW);

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
  config.frame_size = FRAMESIZE_QQVGA; // was FRAMESIZE_UXGA;
  config.pixel_format = PIXFORMAT_GRAYSCALE; // for streaming

#if 0
  config.frame_size = FRAMESIZE_UXGA; // was FRAMESIZE_UXGA;
  config.pixel_format = PIXFORMAT_JPEG;
#endif

#ifdef CAMERA_GRAB_WHEN_EMPTY
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
#endif
  config.jpeg_quality = 12;
  config.fb_count = 1;

#if !defined(CAMERA_FB_IN_PSRAM) || !defined(CAMERA_FB_IN_DRAM)
  // Use PSRAM IC if present.
  //
#if ESP_IDF_VERSION_MAJOR >= 4
  config.fb_location = psramFound() ? CAMERA_FB_IN_PSRAM : CAMERA_FB_IN_DRAM;
#endif
#endif

  // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  //                      for larger pre-allocated frame buffer.
  if (config.pixel_format == PIXFORMAT_JPEG) {
    if (psramFound()) {
      config.jpeg_quality = 10;
      config.fb_count = 2;
#ifdef CAMERA_GRAB_LATEST
      config.grab_mode = CAMERA_GRAB_LATEST;
#endif
    } else {
      config.frame_size = FRAMESIZE_SVGA;
    }
  } else {
#if CONFIG_IDF_TARGET_ESP32S3
    config.fb_count = 2;
#endif
  };

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Log.println("Camera Init Failed");
    return err;
  }

  sensor_t * s = esp_camera_sensor_get();
  //libby
  //s->set_vflip(s, 1);
  //s->set_hmirror(s, 1); 
  
  
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1); // flip it back
    s->set_brightness(s, 1); // up the brightness just a bit
    s->set_saturation(s, -2); // lower the saturation
  }
  // drop down frame size for higher initial frame rate
  if (config.pixel_format == PIXFORMAT_JPEG) {
    s->set_framesize(s, FRAMESIZE_QVGA);
  }

#if defined(CAMERA_MODEL_M5STACK_WIDE) || defined(CAMERA_MODEL_M5STACK_ESP32CAM)
  s->set_vflip(s, 1);
  s->set_hmirror(s, 1);
#endif

#if defined(CAMERA_MODEL_ESP32S3_EYE)
  s->set_vflip(s, 1);
#endif

  s->set_vflip(s, 1);

  Log.println("Cameria initialised");
  return ESP_OK;
}
