/***************************************************************************
  This is a library for the AMG88xx GridEYE 8x8 IR camera

  This sketch tries to read the pixels from the sensor

  Designed specifically to work with the Adafruit AMG88 breakout
  ----> http://www.adafruit.com/products/3538

  These sensors use I2C to communicate. The device's I2C address is 0x69

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Dean Miller for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/

#include <Adafruit_AMG88xx.h>
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
// #include <Wire.h>
#include "JsonConnector.h"
#include "config.h"

WiFiMulti wifiMulti;
// TODO: check what wifiMulti is doing
Adafruit_AMG88xx thermalCam;
HSJsonConnector connector;

float pixels[AMG88xx_PIXEL_ARRAY_SIZE];

// TODO: check wifi status and reconnect if needed

void setup() {
  Serial.begin(115200);
  bool status;

  wifiMulti.addAP(WIFI_SSID, WIFI_PASS);
  connector.setSensor(SENSOR_ID, SENSOR_TYPE);
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

  // wait for WiFi connection
  if ((wifiMulti.run() == WL_CONNECTED)) {
    connector.
  }
  delay(100);
}
