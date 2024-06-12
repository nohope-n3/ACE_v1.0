#include "esp_camera.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

#include "CameraWebServerAccessPoint.h"

// Define pin 33 as RX (receive pin for Serial2)
#define RX_PIN 33
// Define pin 4 as TX (transmit pin for Serial2)
#define TX_PIN 4

// Instantiate CameraWebServer object
CameraWebServerAccessPoint cameraWebServerAccessPoint;

// Function to initialize the setup
void setup() {
    // Initialize serial communication at 115200 baud rate between cam and server
    Serial.begin(115200);
    delay(100);

    // Initialize Serial2 communication at 9600 baud rate with RX and TX pins
    Serial2.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
    
    // Initialize the Camera Web Server Access Point
    cameraWebServerAccessPoint.CameraWebServerAccessPointInit();

    // Print message to serial
    Serial.println("Nohope");
}

void loop() {
}

// // define two tasks
// void handleCamera();
// void handleMessage();

// void setup() {
//     // Initialize serial communication at 115200 baud rate between cam and server
//     Serial.begin(115200);
//     delay(100);

//     // Initialize Serial2 communication at 9600 baud rate with RX and TX pins
//     Serial2.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);

//     // Initialize the Camera Web Server Access Point
//     cameraWebServerAccessPoint.CameraWebServerAccessPointInit();

//     // Print message to serial
//     Serial.println("Nohope");

//     // Now set up two tasks to run independently.
//     xTaskCreatePinnedToCore(
//         handleCamera, "HandleCamera",
//         2048,
//         NULL, 2,
//         NULL, 0);

//     xTaskCreatePinnedToCore(
//         handleMessage, "HandleMessage", 1024,
//         NULL, 1,
//         NULL, 1);
// }

// /*--------------------------------------------------*/
// /*---------------------- Tasks ---------------------*/
// /*--------------------------------------------------*/

// void handleCamera() {
//     httpd_config_t config = HTTPD_DEFAULT_CONFIG();
//     httpd_uri_t capture_uri = {
//         .uri = "/capture",
//         .method = HTTP_GET,
//         .handler = capture_handler,
//         .user_ctx = NULL};

//     Serial.printf("Starting web server on port: '%d'\n", config.server_port);

//     if (httpd_start(&camera_httpd, &config) == ESP_OK) {
//         httpd_register_uri_handler(camera_httpd, &capture_uri);
//     }
// }

// void handleMessage() {
//     httpd_config_t config = HTTPD_DEFAULT_CONFIG();

//     httpd_uri_t message_uri = {
//         .uri = "/message",
//         .method = HTTP_POST,
//         .handler = message_handler,
//         .user_ctx = NULL};

//     config.server_port += 1;
//     config.ctrl_port += 1;
//     Serial.printf("Starting stream server on port: '%d'\n", config.server_port);
//
//     if (httpd_start(&stream_httpd, &config) == ESP_OK) {
//        httpd_register_uri_handler(stream_httpd, &message_uri);
//     }
// }

