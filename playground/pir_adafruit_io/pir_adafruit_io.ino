#include "config.h"

int inputPin = 21;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status

// set up the 'pir0' feed
AdafruitIO_Feed *counter = io.feed("pir1");

void setup() {
  // declare sensor as input
  pinMode(inputPin, INPUT);

  // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  while(! Serial);

  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

}

void loop() {
  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();
  
  val = digitalRead(inputPin);  // read input value
//  if (val == HIGH) {            // check if the input is HIGH
//    if (pirState == LOW) {
//      // we have just turned on
//      Serial.println("Motion detected!");
//      // We only want to print on the output change, not state
//      pirState = HIGH;
//      send(1);
//    }
//  } else {
//    if (pirState == HIGH){
//      // we have just turned of
//      Serial.println("Motion ended!");
//      // We only want to print on the output change, not state
//      pirState = LOW;
//      send(0);
//    }
//  }
  if (val == HIGH) {
    send(1);
  } else {
    send(0);
  }
  delay(2000);
}

void send(int val) {
  // save count to the 'counter' feed on Adafruit IO
  Serial.print("sending -> ");
  Serial.println(val);
  counter->save(val);
}
