#include <Adafruit_AMG88xx.h>
#include <Arduino.h>
#include <HSJsonConnector.h>
#include <WiFi.h>
#include <deque>
#include "config.h"

#define STOVE_HOT_TEMP 50.0
#define STOVE_COLD_TEMP 40.0
#define MOVING_AVG_SIZE 10

enum class StoveState { HOT, COLD, UNKNOWN };

Adafruit_AMG88xx thermalCam;
HSJsonConnector connector;

float pixels[AMG88xx_PIXEL_ARRAY_SIZE];
// Initialize a buffer for moving average
std::deque<float> max_temps;

StoveState stove_state = StoveState::COLD;

void setup() {
  Serial.begin(115200);
  connector.setSensor(SENSOR_ID, SENSOR_TYPE);
  connector.setServer(SERVER_URL);
  thermalCam.begin();
  connectWifi(WIFI_SSID, WIFI_PASS);

  delay(100);  // let sensor boot up
}

void loop() {
  // Read all the pixels
  thermalCam.readPixels(pixels);
  StoveState new_stove_state = getStoveState();
  if (new_stove_state != stove_state) {
    sendData(new_stove_state);
    stove_state = new_stove_state;
  }
  delay(100);
}

StoveState getStoveState() {
  float max_temp = *(std::max_element(pixels, pixels + AMG88xx_PIXEL_ARRAY_SIZE));
  if (max_temps.size() == MOVING_AVG_SIZE) {
    // The max_temps buffer is full

  }
  int count = 0;
  for (int k = 1; k <= 3; k++) {
    for (int j = 0; j <= AMG88xx_PIXEL_ARRAY_SIZE - 1; j++) {
      if (pixels[j] > 30.0) {
        count = count + 1;
        break;
      }
    }
    delay(1000);
  }
  if (count == 3) {
    Serial.println("Stove is on");
  }

  count = 0;
  return StoveState::UNKNOWN;
}

void connectWifi(const char* ssid, const char* password) {
  Serial.print("Connecting Wifi");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void sendData(StoveState status) {
  String pixels_string;
  for (int i = 0; i < AMG88xx_PIXEL_ARRAY_SIZE; i++) {
    pixels_string += (pixels_string == NULL ? "" : ",") + String(pixels[i], 2);
  }
  String data;
  switch (status) {
    case StoveState::HOT:
      data = "[{\"message\":\"STOVE_HOT\", \"value\":[" + pixels_string + "]}]";
      break;
    case StoveState::COLD:
      data =
          "[{\"message\":\"STOVE_COLD\", \"value\":[" + pixels_string + "]}]";
      break;
  }

  if (true) {
    // TODO: check wifi connection
    connector.send(HSEvent::DATA, data);
  }
}
