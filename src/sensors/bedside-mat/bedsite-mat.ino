#include <HSJsonConnector.h>


#include <ArduinoJson.h>
//#include <HSJsonConnector.h>


#define SENSOR_ID "7eeee37c-18f0-4633-91c3-88822375f646"
//#define WIFI_SSID "AndroidAPDEB3"
//#define WIFI_PASS "vvmp5133"
#define WIFI_SSID "CMU-DEVICE"
#define WIFI_PASS ""
#define SENSOR_TYPE "pressure_mat"
#define SERVER_URL "http://kinect.andrew.cmu.edu:8000/hs/events"
#define SERVER_AUTH "Basic aHNfdXNlcjpycGNzX2hzMjAxOQ=="

const int thresh = 150;
//Mux control pins for analog signal (SIG_pin) default for arduino mini pro
const byte s0 = 21;//34;
const byte s1 = 18;//39;
const byte s2 = 19;//36;
const byte s3 = 23;//4;

//Mux control pins for Output signal (OUT_pin) default for arduino mini pro
const byte w0 = 27; 
const byte w1 = 33;
const byte w2 = 15;
const byte w3 = 32;

//Mux in "SIG" pin default for arduino mini pro 
const byte SIG_pin = A2; 

//Mux out "SIG" pin default for arduino mini pro
const byte OUT_pin = 14;

//Row and Column pins default for arduino mini pro
const byte STATUS_pin = 8;
const byte COL_pin = 9;

const boolean muxChannel[16][4]={
    {0,0,0,0}, //channel 0
    {1,0,0,0}, //channel 1
    {0,1,0,0}, //channel 2
    {1,1,0,0}, //channel 3
    {0,0,1,0}, //channel 4
    {1,0,1,0}, //channel 5
    {0,1,1,0}, //channel 6
    {1,1,1,0}, //channel 7
    {0,0,0,1}, //channel 8
    {1,0,0,1}, //channel 9
    {0,1,0,1}, //channel 10
    {1,1,0,1}, //channel 11
    {0,0,1,1}, //channel 12
    {1,0,1,1}, //channel 13
    {0,1,1,1}, //channel 14
    {1,1,1,1}  //channel 15
  };


//incoming serial byte
int inByte = 0;

int valor = 0;               //variable for sending bytes to processing
int calibra[15][15];         //Calibration array for the min values of each od the 225 sensors.
int minsensor=254;          //Variable for staring the min array
int multiplier = 254;
int pastmatrix[15][15];

HSJsonConnector connector;


void setup(){
    
  pinMode(s0, OUTPUT); 
  pinMode(s1, OUTPUT); 
  pinMode(s2, OUTPUT); 
  pinMode(s3, OUTPUT); 
  
  pinMode(w0, OUTPUT); 
  pinMode(w1, OUTPUT); 
  pinMode(w2, OUTPUT); 
  pinMode(w3, OUTPUT); 
  
  pinMode(OUT_pin, OUTPUT); 
  pinMode(SIG_pin, INPUT); 
  pinMode(STATUS_pin, OUTPUT);
  pinMode(COL_pin, OUTPUT);

  
  digitalWrite(s0, LOW);
  digitalWrite(s1, LOW);
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);
  
  digitalWrite(w0, LOW);
  digitalWrite(w1, LOW);
  digitalWrite(w2, LOW);
  digitalWrite(w3, LOW);
  
  digitalWrite(OUT_pin, HIGH);
  digitalWrite(STATUS_pin, HIGH);
  digitalWrite(COL_pin, HIGH);
  
  connector.setSensor(SENSOR_ID, SENSOR_TYPE);
  connector.setServer(SERVER_URL);
  #ifdef SERVER_AUTH
  connector.setServerAuth(SERVER_AUTH);
  #endif
  
  connectWifi(WIFI_SSID, WIFI_PASS);

  delay(100);

  Serial.begin(115200);
  Serial.println("asdfg");
  Serial.println("\nCalibrating...\n");


  digitalWrite(COL_pin, LOW);
}




void loop(){
  //Loop through and read all 16 values
  //Reports back Value at channel 6 is: 346
    int count = 0;
    int max_value = 0;
    String data;
    String value_string;
    
      for(int j = 0; j < 5; j++){ 
        writeMux(j);
        
        for(int i = 0; i < 10; i++){
          valor = readMux(i);
          
          //Saturation sensors
          if (valor > thresh) {
            count++;
          }
          if (valor > max_value) {
            max_value = valor;
          }
          Serial.print(valor);
          Serial.print("\t");
          value_string += (value_string == NULL ? "" : ",") + String(valor);
          digitalWrite(COL_pin,!digitalRead(COL_pin));
        } 
        Serial.println();
        delay(50);
      }
      Serial.println();
      Serial.println();
      if (count > 19 || max_value > 685) {
        Serial.println("FEET DETECTED!");
        data = "{\"message\":\"FEET_DETECTED\", \"value\":[" + value_string + "]}";
        connector.send(HSEvent::DATA, data);
        delay(10000);
      }


}




int readMux(byte channel){
  byte controlPin[] = {s0, s1, s2, s3};

  //loop through the 4 sig
  for(int i = 0; i < 4; i ++){
    digitalWrite(controlPin[i], muxChannel[channel][i]);
  }

  //read the value at the SIG pin
  int val = analogRead(SIG_pin);

  //return the value
  return val;
}



void writeMux(byte channel){
  byte controlPin[] = {w0, w1, w2, w3};

  //loop through the 4 sig
  for(byte i = 0; i < 4; i ++){
    digitalWrite(controlPin[i], muxChannel[channel][i]);
  }
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