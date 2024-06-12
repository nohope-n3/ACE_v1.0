#define CAMERA_MODEL_M5STACK_WIDE

#include "CameraPins.h"
#include "esp_system.h"

#include "CameraWebServerAccessPoint.h"

// Start Camera Server
void startCameraServer();

// Initialize the camera web server
void CameraWebServerAccessPoint::CameraWebServerAccessPointInit() {
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
    config.xclk_freq_hz = 10000000; // 10 MHz
    config.pixel_format = PIXFORMAT_JPEG;

    config.frame_size = FRAMESIZE_XGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;

#if defined(CAMERA_MODEL_ESP_EYE)
    pinMode(13, INPUT_PULLUP);
    pinMode(14, INPUT_PULLUP);
#endif
    // Camera initialization
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x", err);
        return;
    }

    sensor_t *pSensor = esp_camera_sensor_get();

    // Drop down frame size for higher initial frame rate
    pSensor->set_framesize(pSensor, FRAMESIZE_VGA);

#if defined(CAMERA_MODEL_M5STACK_WIDE)
    pSensor->set_vflip(pSensor, 1);
    pSensor->set_hmirror(pSensor, 1);
#endif
    // Image direction settings (up and down)
    pSensor->set_vflip(pSensor, 1);
    // Image direction settings (left and right)
    pSensor->set_hmirror(pSensor, 1);

    // Connect to wifi
    // WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("\nConnecting");

    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(100);
    }

    Serial.println("\nConnected to the WiFi network");
    Serial.print("SSID: ");
    Serial.println(ssid);
    Serial.print("Password: ");
    Serial.println(password);

    // Start camera server
    startCameraServer();

    // Serial print camera ready message
    Serial.print("Camera Ready! Use 'http://");
    Serial.print(WiFi.localIP());
    Serial.println("' to connect");
}
