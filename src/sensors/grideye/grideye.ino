#include <Adafruit_AMG88xx.h>
#include <Arduino.h>
#include <HSJsonConnector.h>
#include <WiFi.h>
#include <deque>
#include "config.h"

#define STOVE_HOT_TEMP 43.0
#define STOVE_COLD_TEMP 40.0
#define MOVING_AVG_SIZE 10
#define REFRESH_RATE 500

enum class StoveState { HOT, COLD, WARM, UNKNOWN };

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
  if (new_stove_state != stove_state && new_stove_state != StoveState::WARM) {
    sendData(new_stove_state);
    stove_state = new_stove_state;
  }
  delay(REFRESH_RATE);
}

StoveState getStoveState() {
  // Store the current max temperature to the buffer
  float maxtemp = *(std::max_element(pixels, pixels + AMG88xx_PIXEL_ARRAY_SIZE));
  if (max_temps.size() == MOVING_AVG_SIZE) {
    // The max_temps buffer is full, pop the oldest one
    max_temps.pop_front();
  }
  max_temps.push_back(maxtemp);
  // Find a moving average
  float avg = std::accumulate(max_temps.begin(), max_temps.end(), 0.0) / (float)max_temps.size();
  Serial.printf("avg %g\n", avg);
  if (avg > STOVE_HOT_TEMP) {
    return StoveState::HOT;
  }
  if (STOVE_COLD_TEMP < avg && avg < STOVE_HOT_TEMP) {
    return StoveState::WARM;
  }
  return StoveState::COLD;
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
