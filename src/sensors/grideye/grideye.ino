#include <Adafruit_AMG88xx.h>
#include <Arduino.h>
#include <WiFi.h>
#include "HSJsonConnector.h"
#include "config.h"

using namespace std;

Adafruit_AMG88xx thermalCam;
HSJsonConnector connector;

float pixels[AMG88xx_PIXEL_ARRAY_SIZE];

void setup() {
  Serial.begin(115200);
  connector.setSensor(SENSOR_ID, SENSOR_TYPE);
  connector.setServer(SERVER_URL);
  thermalCam.begin();

  Serial.print("Connecting Wifi");
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // read all the pixels
  thermalCam.readPixels(pixels);

  String pixels_string;
  for (int i = 0; i < AMG88xx_PIXEL_ARRAY_SIZE; i++) {
    pixels_string += (pixels_string==NULL ? "" : ",") + String(pixels[i], 2);
  }

  String data = "{message:\"STOVE_HOT\", value:[" + pixels_string + "]}";
  if (true) {
    // TODO: check wifi connection
    connector.send(HSEvent::DATA, data);
  }
  delay(100);
}
