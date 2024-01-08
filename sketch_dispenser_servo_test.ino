#include <ESP32Servo.h>

Servo myservo;  // create servo object to control a servo
// 16 servo objects can be created on the ESP32
int pos = 0; 
int servoPin = 32;

void setup() {
  Serial.begin(9600);
  pinMode(2, OUTPUT); //Test pin
  pinMode(32, OUTPUT); //De dispenser motor is verbonden op pin32.
  // Allow allocation of all timers
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	myservo.setPeriodHertz(50);    // standard 50 hz servo
	myservo.attach(servoPin, 1000, 3000); // attaches the servo on pin 18 to the servo object
	// using default min/max of 1000us and 2000us
	// different servos may require different min/max settings
	// for an accurate 0 to 180 sweep
}

void loop() {
  Serial.println("hallootjes4");
  //for (pos = 0; pos <= 90; pos += 2) { // goes from 0 degrees to 180 degrees
  //   // in steps of 1 degree
  //    myservo.write(pos);    // tell servo to go to position in variable 'pos'
  //    delay(15);             // waits 15ms for the servo to reach the position
  //}
  //for (pos = 90; pos >= 0; pos -= 2) { // goes from 180 degrees to 0 degrees
  //     myservo.write(pos);    // tell servo to go to position in variable 'pos'
  //    delay(15);             // waits 15ms for the servo to reach the position
  //}
  myservo.write(180);
  delay(1000);
  myservo.write(0);
  delay(5000);
}