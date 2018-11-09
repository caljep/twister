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
  debug = false;
  myServo.attach(servoPin);
  Particle.function("toggleDebug",toggleDebug);
  
  Serial.printlnf("Boot-up time: %s", Time.timeStr().c_str());
  initialTime = Time.now(); 
  initialMillis = millis();

  attachInterrupt(D2, setInterruptTime, CHANGE);
}

void loop() {
  newReading = map(analogRead(potPin),0,4095,0,180);

  //To smooth out the noise in the analog reading, I'm taking an avg
  readings[counter % size] = newReading;
  counter++;
  accum = 0;
  for (int i=0; i<size; i++) {
    accum += readings[i];
  }
  //Write the avg value of the Pot. to the servo
  myServo.write(accum/size);

  //Reading the interrupt time on the D2 pin and printing it to serial. 
  if (lastTime != interruptTimeRelative) {
    Serial.printf("Latest Interrupt: %s\n", Time.timeStr(interruptTimeRelative/1000+initialTime).c_str());
    lastTime = interruptTimeRelative;
  }

  delay(30);
}