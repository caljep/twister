#include "application.h"
#line 1 "/Users/calparticle/Desktop/repos/twister/src/twister.ino"
/*
 * Project twister
 * Description:
 * Author:
 * Dce:
 */
int toggleDebug(String command);
void setInterruptTime();
void setup();
void loop();
#line 7 "/Users/calparticle/Desktop/repos/twister/src/twister.ino"
int potPin = A7, servoPin = A4, counter = 0;
int readings [6];

bool debug;
int newReading;
int size = sizeof(readings)/sizeof(readings[0]);
int accum;

time_t initialTime;
time_t initialMillis;
time_t lastTime = 0;
volatile time_t interruptTimeRelative = 0;

Servo myServo;

int toggleDebug(String command) {
  debug = !debug;
  int ret = debug ? 1 : 0;
  return ret;
}

void setInterruptTime() {
  interruptTimeRelative = millis() - initialMillis;
}

// setup() runs once, when the device is first turned on.
void setup() {
  // Put initialization like pinMode and begin functions here.
  debug = false;
  myServo.attach(servoPin);
  Particle.function("toggleDebug",toggleDebug);
  
  Serial.printlnf("Boot-up time: %s", Time.timeStr().c_str());
  initialTime = Time.now(); 
  initialMillis = millis();

  attachInterrupt(D2, setInterruptTime, CHANGE);
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  newReading = map(analogRead(potPin),0,4095,0,180);
  readings[counter % size] = newReading;
  counter++;

  // if (debug) {
  //   Serial.printf("size: %d, counter: %d\n",size, counter);
  //   Serial.printf("nr: %d\n",newReading);
  //   Serial.printf("%d, %d, %d, %d\n", readings[0], readings[1], readings[2], readings[3]);
  //   Serial.printf("avg: %i\n", accum/size);
  // }
  
  accum = 0;
  for (int i=0; i<size; i++) {
    accum += readings[i];
  }
  //Serial.printlnf("Current time)
  if (lastTime != interruptTimeRelative) {
    Serial.printf("Latest Interrupt: %s\n", Time.timeStr(interruptTimeRelative/1000+initialTime).c_str());
    lastTime = interruptTimeRelative;
  }
  // Serial.printf("current time: %s\n",Time.timeStr(initialTime + interruptTimeRelative/1000));
  myServo.write(accum/size);
  delay(30);
}