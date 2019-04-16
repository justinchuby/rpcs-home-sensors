#include <Adafruit_AMG88xx.h>
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
// #include <fmt/core.h>
#include <string>
#include <array>
#include "JsonConnector.h"
#include "config.h"

using namespace std;

WiFiMulti wifiMulti;
// TODO: check what wifiMulti is doing
Adafruit_AMG88xx thermalCam;
HSJsonConnector connector;

array<float, AMG88xx_PIXEL_ARRAY_SIZE> pixels;
// float pixels[AMG88xx_PIXEL_ARRAY_SIZE];

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

  // for (int i = 0; i < AMG88xx_PIXEL_ARRAY_SIZE; i++) {
  //   Serial.print(pixels[i]);
  //   Serial.print(",");
  // }
  // // End each frame with a linefeed
  // Serial.println();

  // TODO: use logic to tell stove hot or stove code
  string pixels_string;
  for (float v : pixels) {
    pixels_string += (pixels_string.empty() ? "" : ",") + to_string(v);
  }
  // for (int i = 0; i < AMG88xx_PIXEL_ARRAY_SIZE; i++) {
  //   pixels_string += fmt::format("{:.2f}", pixels[i]);
  // }
  string data = "{message:\"STOVE_HOT\", value:[" + pixels_string + "]}";
  if ((wifiMulti.run() == WL_CONNECTED)) {
    // Wait for WiFi connection
    connector.send("data", data);
  }
  delay(100);
}
