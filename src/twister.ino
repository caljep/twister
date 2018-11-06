/*
 * Project twister
 * Description:
 * Author:
 * Date:
 */
int potPin = A0, servoPin = D1, counter = 0;
bool debug;
int reading, servoPosition;
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
  reading = analogRead(potPin);
  servoPosition = map(reading,0,4095,0,180);
  if (debug) {
    Serial.printf("Potentiometer: %i, ServoPosition: %i\n",reading, servoPosition);
    delay(1000);
  }
  myServo.write(servoPosition);
  delay(30);
}