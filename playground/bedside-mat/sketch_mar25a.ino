const byte s0 = A5;
//const byte s1 = A6;

//Row and Column pins default for arduino mini pro
const byte STATUS_pin = 8;
const byte COL_pin = 9;

void setup() {
  // put your setup code here, to run once:
  pinMode(s0, INPUT); 
//  pinMode(s1, INPUT); 
  Serial.begin(115200);
  Serial.println("Testing");
}

void loop() {
  // put your main code here, to run repeatedly:
//  if (Serial.available() > 0){
  int inByte = analogRead(A5);
  Serial.println(inByte);
  delay(500);
//  }
}
