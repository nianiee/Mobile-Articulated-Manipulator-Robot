#include <IRremote.h>
// C++ code ultrasonic sensor 
const int trig = 6;
const int echo = 5;
int duration;
int distance;
int t1 = 300;
int t2 = 300;
const int stopDistance = 25; 

// IR
const int IRpin = 10;
IRrecv IR(IRpin);

// motor 1
const int dir1 = 2;
const int dir2 = 4;
const int speed1 = 3; // ENA

// motor 2
const int dir3 = 8; // Corrected pin assignment for motor 2
const int dir4 = 12; // Corrected pin assignment for motor 2
const int speed2 = 9; // ENB 

int motSpeed1 = 255;
int motSpeed2 = 0; // Set to the same speed as motSpeed1

void setup() { 
  Serial.begin(9600);
  IR.enableIRIn();
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  // motor 1
  pinMode(dir1, OUTPUT);
  pinMode(dir2, OUTPUT);
  pinMode(speed1, OUTPUT);

  // motor 2
  pinMode(dir3, OUTPUT);
  pinMode(dir4, OUTPUT);
  pinMode(speed2, OUTPUT); // ENB for motor 2

  // Stop the motors at startup
  digitalWrite(dir1, LOW);
  digitalWrite(dir2, LOW);
  digitalWrite(dir3, LOW);
  digitalWrite(dir4, LOW);
  analogWrite(speed1, 0);
  analogWrite(speed2, 0);
}

void loop() {
  digitalWrite(trig, LOW);
  delayMicroseconds(10);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  duration = pulseIn(echo, HIGH);
  distance = (duration / 2) / 29.1;

  // Display the distance and stop indicator on the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm");

  while (IR.decode()) // For decoding buttons
  {
    if (IR.decodedIRData.decodedRawData == 0xB946FF00) // #2 button
    {
      // Motors direction - forward 
      digitalWrite(dir1, LOW);
      digitalWrite(dir2, HIGH);
      digitalWrite(dir3, LOW);
      digitalWrite(dir4, HIGH);
      analogWrite(speed1, motSpeed1);
      analogWrite(speed2, motSpeed1);
      delay(t1);
    }
    else if (IR.decodedIRData.decodedRawData == 0xEA15FF00) // #8 button
    {
      // Motors direction - backward
      digitalWrite(dir1, HIGH);
      digitalWrite(dir2, LOW);
      digitalWrite(dir3, HIGH);
      digitalWrite(dir4, LOW);
      analogWrite(speed1, motSpeed1);
      analogWrite(speed2, motSpeed1);
      delay(t1);
    }
    else if (IR.decodedIRData.decodedRawData == 0xBB44FF00) // #4 button
    {
      // Motors direction - left
      digitalWrite(dir1, HIGH);
      digitalWrite(dir2, LOW);
      digitalWrite(dir3, HIGH);
      digitalWrite(dir4, LOW);
      analogWrite(speed1, motSpeed1);
      analogWrite(speed2, motSpeed2);
      delay(t1);
    }
    else if (IR.decodedIRData.decodedRawData == 0xBC43FF00) // #6 button
    {
      // Motors direction - right
      digitalWrite(dir1, HIGH);
      digitalWrite(dir2, LOW);
      digitalWrite(dir3, HIGH);
      digitalWrite(dir4, LOW);
      analogWrite(speed1, motSpeed2);
      analogWrite(speed2, motSpeed1);
      delay(t1);
    }
    IR.resume();
    delay(100); // Add a small delay between IR.decode() iterations
  }

  if (distance <= stopDistance) 
  {
    Serial.println(" - Stop");
    
    // Motors direction - stop 
    digitalWrite(dir1, LOW);
    digitalWrite(dir2, LOW);
    digitalWrite(dir3, LOW);
    digitalWrite(dir4, LOW);
    analogWrite(speed1, 0); // Stop motor 1
    analogWrite(speed2, 0); // Stop motor 2
  } 
}