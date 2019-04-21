#include <Adafruit_AMG88xx.h>
#include <Arduino.h>
#include <HSJsonConnector.h>
#include <WiFi.h>
#include "config.h"

#define STOVE_HOT_TEMP 50.0
#define STOVE_COLD_TEMP 40.0
#define MOVING_AVG_SIZE 10

Adafruit_AMG88xx thermalCam;
HSJsonConnector connector;

float pixels[AMG88xx_PIXEL_ARRAY_SIZE];
float max_temps[MOVING_AVG_SIZE];

struct 

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

  delay(100);  // let sensor boot up
}

void loop() {
  // Read all the pixels
  thermalCam.readPixels(pixels);



  String pixels_string;
  for (int i = 0; i < AMG88xx_PIXEL_ARRAY_SIZE; i++) {
    pixels_string += (pixels_string == NULL ? "" : ",") + String(pixels[i], 2);
  }

  String data =
      "[{\"message\":\"STOVE_HOT\", \"value\":[" + pixels_string + "]}]";
  if (true) {
    // TODO: check wifi connection
    connector.send(HSEvent::DATA, data);
  }
  delay(100);
}

bool stove_hot(float* max_temps) {
  int count = 0;
  for (int k = 1; k <= 3; k++) {
    for (int j = 0; j <= AMG88xx_PIXEL_ARRAY_SIZE - 1; j++) {
      if (pixels[j] > 30.0) {
        count = count + 1;
        break;
      }
    }
    delay(1000);
    amg.readPixels(pixels);
  }
  if (count == 3) {
    Serial.println("Stove is on");
  }

  count = 0;
}