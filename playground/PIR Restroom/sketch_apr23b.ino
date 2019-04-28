#include "config.h"
#include <HSJsonConnector.h>

// 1 Minute
#define HANDSHAKE_INTERVAL 60000
// 10 seconds
#define DATA_INTERVAL 10000
//Sensor ID
// #define SENSOR_ID 500
//Sensor type
// #define SENSOR_TYPE "PIR"
//Auth string
#define SERVER_AUTH "Basic aHNfdXNlcjpycGNzX2hzMjAxOQ=="
//Server URL
#define SERVER_URL "http://kinect.andrew.cmu.edu:8000/hs/events"
//Wifi SSID
#define WIFI_SSID "CMU-DEVICE"
//Wifi password
#define WIFI_PASS ""

#define REFRESH_RATE 500

int inputPin = 21;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status

// set up the 'pir0' feed
//AdafruitIO_Feed *counter = io.feed("pir1");

HSJsonConnector connector;
int time_count_handshake = 0;
int time_count_data = 0;

const char* SENSOR_ID = "c76c5d12-6647-11e9-a923-1681be663d3e";
const char* SENSOR_TYPE = "pir";
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

  // wait for serial monitor to open
//  while(! Serial);

//  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
//  io.connect();

  // wait for a connection
//  while(io.status() < AIO_CONNECTED) {
//    Serial.print(".");
//    delay(500);
//  }

  // we are connected
  Serial.println();
//  Serial.println(io.statusText());

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
//      // we have just turned on
      Serial.println("Motion detected!");
//      // We only want to print on the output change, not state
      pirState = HIGH;
//      send(1);
    sendData(1);
    }
  } 
  else {
    if (pirState == HIGH){
//      // we have just turned of
      Serial.println("Motion ended!");
//      // We only want to print on the output change, not state
      pirState = LOW;
//      send(0);
    sendData(0);
    }
  }
//  delay(2000);
delay(REFRESH_RATE);

  time_count_handshake += REFRESH_RATE;
  time_count_data += REFRESH_RATE;
  if (time_count_handshake > HANDSHAKE_INTERVAL) {
    sendHandshake();
    time_count_handshake = 0;
  }
  if (time_count_data > DATA_INTERVAL) {
    // sendData(getStoveState());
    // DEBUG: send data every 10 secs
    sendData(2);
    time_count_data = 0;
  }
}

void sendData(int val) {
  String data;
  switch (val) {
    case 0:
      data = "{\"message\":\"MOTION ENDED\", \"value\":0}";
      break;
    case 1:
      data =
          "{\"message\":\"MOTION DETECTED\", \"value\":1}";
      break;
    case 2:
      data =
          "{\"message\":\"UNKNOWN\", \"value\":2}";
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
