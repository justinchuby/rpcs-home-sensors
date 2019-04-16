#include <Adafruit_AMG88xx.h>
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include "HSJsonConnector/HSJsonConnector.h"
#include "config.h"

using namespace std;

WiFiMulti wifiMulti;
// TODO: check what wifiMulti is doing
Adafruit_AMG88xx thermalCam;
HSJsonConnector connector;

float pixels[AMG88xx_PIXEL_ARRAY_SIZE];

void setup() {
  Serial.begin(115200);
  bool status;

  wifiMulti.addAP(WIFI_SSID, WIFI_PASS);
  connector.setSensor(SENSOR_ID, SENSOR_TYPE);
  connector.setServer(SERVER_URL);
  thermalCam.begin();
}

void loop() {
  // read all the pixels
  thermalCam.readPixels(pixels);

  String pixels_string;
  for (int i = 0; i < AMG88xx_PIXEL_ARRAY_SIZE; i++) {
    pixels_string += (pixels_string==NULL ? "" : ",") + String(pixels[i], 2);
  }
  // // End each frame with a linefeed
  // Serial.println();

  // TODO: use logic to tell stove hot or stove code
//  string pixels_string;
//  for (float v : pixels) {
//    pixels_string += (pixels_string.empty() ? "" : ",") + to_string(v);
//  }

  String data = "{message:\"STOVE_HOT\", value:[" + pixels_string + "]}";
  if ((wifiMulti.run() == WL_CONNECTED)) {
    // Wait for WiFi connection
    connector.send(HSEvent::DATA, data);
  }
  delay(100);
}
