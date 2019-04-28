#include "config.h"
#include <HSJsonConnector.h>

// 1 Minute
#define HANDSHAKE_INTERVAL 60000
#define REFRESH_RATE 500

int inputPin = 21;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status

// set up the 'pir0' feed
//AdafruitIO_Feed *counter = io.feed("pir1");

HSJsonConnector connector;
int time_count_handshake = 0;

void setup() {
  connector.setSensor(SENSOR_ID, SENSOR_TYPE);
  connector.setServer(SERVER_URL);
  #ifdef SERVER_AUTH
  connector.setServerAuth(SERVER_AUTH);
  #endif
  // declare sensor as input
  pinMode(inputPin, INPUT);

  // start the serial connection
  Serial.begin(115200);

  // we are connected
  Serial.println();
  connectWifi(WIFI_SSID, WIFI_PASS);

  delay(100);  // let sensor boot up
}

void loop() {
  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
//  io.run();
  
  val = digitalRead(inputPin);  // read input value
  if (val == HIGH) {            // check if the input is HIGH
    if (pirState == LOW) {
      Serial.println("Motion detected!");
      pirState = HIGH;
      sendData(1);
    }
  } 
  else {
    if (pirState == HIGH){
      Serial.println("Motion ended!");
      pirState = LOW;
      sendData(0);
    }
  }
  delay(REFRESH_RATE);
  time_count_handshake += REFRESH_RATE;
  time_count_data += REFRESH_RATE;
  if (time_count_handshake > HANDSHAKE_INTERVAL) {
    sendHandshake();
    time_count_handshake = 0;
  }
}

void sendData(int val) {
  String data;
  switch (val) {
    case 0:
      data = "{\"message\":\"MOTION_ENDED\", \"value\": 0}";
      break;
    case 1:
      data = "{\"message\":\"MOTION_DETECTED\", \"value\": 1}";
      break;
    case -1:
      data = "{\"message\":\"UNKNOWN\", \"value\": -1}";
      break;
  }
  if (true) {
    // TODO: check wifi connection
    connector.send(HSEvent::DATA, data);
  }
}

void connectWifi(const char* ssid, const char* password) {
  Serial.print("Connecting Wifi");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void sendHandshake() {
  connector.send(HSEvent::HANDSHAKE, "[]");
}
