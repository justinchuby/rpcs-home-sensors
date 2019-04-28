/*
 * 
 * All the resources for this project: https://randomnerdtutorials.com/
 * Modified by Rui Santos
 * 
 * Created by FILIPEFLOP
 * 
 */
 
#include <SPI.h>
#include <MFRC522.h>

#include "config.h"
#include <HSJsonConnector.h>

// 1 Minute
#define HANDSHAKE_INTERVAL 60000
// 10 seconds
#define DATA_INTERVAL 10000
//Sensor ID
// #define SENSOR_ID 500
//Sensor type
// #define SENSOR_TYPE "RFIDMAIN"
//Auth string
#define SERVER_AUTH "Basic aHNfdXNlcjpycGNzX2hzMjAxOQ=="
//Server URL
#define SERVER_URL "http://kinect.andrew.cmu.edu:8000/hs/events"
//Wifi SSID
#define WIFI_SSID "CMU-DEVICE"
//Wifi password
#define WIFI_PASS ""

#define REFRESH_RATE 500
 
#define SS_PIN 23
#define RST_PIN 22
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

//#define SENSORPIN A0
 
// variables will change:
//int sensorState = 0; //lastState=0;         // variable for reading the pushbutton status

HSJsonConnector connector;
int time_count_handshake = 0;
int time_count_data = 0;

const char* SENSOR_ID = "c26e4e88-2ecb-42ff-8482-4af80307a485";
const char* SENSOR_TYPE = "rfidrest";
 
void setup() 
{
  connector.setSensor(SENSOR_ID, SENSOR_TYPE);
  connector.setServer(SERVER_URL);
  #ifdef SERVER_AUTH
  connector.setServerAuth(SERVER_AUTH);
  #endif
 // pinMode(SENSORPIN, INPUT);     
 // digitalWrite(SENSORPIN, HIGH);
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();
  connectWifi(WIFI_SSID, WIFI_PASS);
  delay(100);  // let sensor boot up

}
void loop() 
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();

  if(content.substring(1) == "04 6F CA 62 E7 4C 80"||content.substring(1) == "04 6F CA 62 E7 4C 81")
        {
           Serial.println("Patient near restroom, check PIR");
           Serial.println();
           delay(3000);
           sendData(1);
        }

  
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
    case 1:
      data = "{\"message\":\"Patient near restroom, check PIR\", \"value\":[]}";
      break;
    case 2:
      data = "{\"message\":\"UNKNOWN\", \"value\":[]}";
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
