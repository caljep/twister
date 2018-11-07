/*
 * Project twister
 * Description:
 * Author:
 * Dce:
 */
int potPin = A7, servoPin = A4, counter = 0;
int readings [6];

bool debug;
int newReading;
int size = sizeof(readings)/sizeof(readings[0]);
int accum;

Servo myServo;

int toggleDebug(String command) {
  debug = !debug;
  int ret = debug ? 1 : 0;
  return ret;
}

// setup() runs once, when the device is first turned on.
void setup() {
  // Put initialization like pinMode and begin functions here.
  debug = false;
  myServo.attach(servoPin);
  Particle.function("toggleDebug",toggleDebug);
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  newReading = map(analogRead(potPin),0,4095,0,180);
  readings[counter % size] = newReading;
  counter++;

  // if (debug) {
  // Serial.printf("size: %d, counter: %d\n",size, counter);
  // Serial.printf("nr: %d\n",newReading);
  // Serial.printf("%d, %d, %d, %d\n", readings[0], readings[1], readings[2], readings[3]);
  // }
  
  accum = 0;
  for (int i=0; i<size; i++) {
    accum += readings[i];
  }
  Serial.printf("avg: %i\n", accum/size);
  myServo.write(accum/size);
  delay(30);
}