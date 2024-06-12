#ifndef _CameraWebServer_AP_H
#define _CameraWebServer_AP_H

#include "esp_camera.h"
#include <WiFi.h>

class CameraWebServerAccessPoint {
  public:
    // Initialize the camera web server
    void CameraWebServerAccessPointInit();
    
  private:
    const char *ssid = "P109";
    const char *password = "12345678";
};

#endif
